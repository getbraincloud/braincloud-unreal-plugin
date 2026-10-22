// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCPortalOAuthClient.h"
#include "BCWidgetPrivatePCH.h"
#include "HAL/PlatformProcess.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Async/Async.h"
#include "Runtime/Launch/Resources/Version.h"

#if BC_WIDGET_OAUTH_SUPPORTED
#include "HttpPath.h"
#include "HttpServerModule.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "IHttpRouter.h"
#endif

THIRD_PARTY_INCLUDES_START
#include "braincloud/BrainCloudNativeWriter.h"
THIRD_PARTY_INCLUDES_END

namespace
{
    std::string ToNative(const FString& Value)
    {
        FTCHARToUTF8 Converted(*Value);
        return std::string(Converted.Get(), Converted.Length());
    }

    FString FromNative(const std::string& Value)
    {
        return FString(UTF8_TO_TCHAR(Value.c_str()));
    }

    void SendPortalRequest(const braincloud::PortalRequest& PortalRequest, TFunction<void(int32, const std::string&)> OnResponse)
    {
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
        TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif
        Request->SetURL(FromNative(PortalRequest.url));
        Request->SetVerb(FromNative(PortalRequest.method));
        for (const std::pair<std::string, std::string>& Header : PortalRequest.headers)
        {
            Request->SetHeader(FromNative(Header.first), FromNative(Header.second));
        }
        if (!PortalRequest.body.empty() || PortalRequest.method == "POST")
        {
            Request->SetContentAsString(FromNative(PortalRequest.body));
        }

        Request->OnProcessRequestComplete().BindLambda(
            [OnResponse](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
            {
                if (!bConnectedSuccessfully || !Response.IsValid())
                {
                    OnResponse(0, std::string());
                    return;
                }
                const TArray<uint8>& Content = Response->GetContent();
                OnResponse(Response->GetResponseCode(), std::string(reinterpret_cast<const char*>(Content.GetData()), Content.Num()));
            });
        Request->ProcessRequest();
    }

#if BC_WIDGET_OAUTH_SUPPORTED
    template <typename LambdaType>
    auto MakeHttpRequestHandler(LambdaType&& InLambda, int) -> decltype(FHttpRequestHandler::CreateLambda(Forward<LambdaType>(InLambda)))
    {
        return FHttpRequestHandler::CreateLambda(Forward<LambdaType>(InLambda));
    }

    template <typename LambdaType>
    FHttpRequestHandler MakeHttpRequestHandler(LambdaType&& InLambda, ...)
    {
        return FHttpRequestHandler(Forward<LambdaType>(InLambda));
    }
#endif
}

BCPortalOAuthClient::BCPortalOAuthClient()
{
}

BCPortalOAuthClient::~BCPortalOAuthClient()
{
#if BC_WIDGET_OAUTH_SUPPORTED
    StopListening();
#endif
}

void BCPortalOAuthClient::StartLogin(const FString& InServerUrl, const FString& InClientId,
    FBCLoginSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure)
{
    OnSuccessDelegate = OnSuccess;
    OnFailureDelegate = OnFailure;

#if !BC_WIDGET_OAUTH_SUPPORTED
    Fail(TEXT("OAuth login requires Unreal Engine 4.24 or later (HTTPServer module)."));
#else
    ServerUrl = InServerUrl;
    Auth = MakeUnique<braincloud::PortalAuth>();
    const FString AuthorizeUrl = FromNative(Auth->Begin(ToNative(ServerUrl), ToNative(InClientId)));
    if (AuthorizeUrl.IsEmpty())
    {
        Fail(TEXT("Could not start the brainCloud portal login."));
        return;
    }

    StopListening();

    const int32 ListenPort = braincloud::PortalAuth::kRedirectPort;
    FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
    Router = HttpServerModule.GetHttpRouter(ListenPort, false);
    if (!Router.IsValid())
    {
        Fail(FString::Printf(TEXT("Could not open the OAuth redirect listener on port %d - is another Unreal Editor already logging in?"), ListenPort));
        return;
    }

    RouteHandle = Router->BindRoute(FHttpPath(UTF8_TO_TCHAR(braincloud::PortalAuth::RedirectPath())), EHttpServerRequestVerbs::VERB_GET,
        MakeHttpRequestHandler(
            [WeakThis = TWeakPtr<BCPortalOAuthClient>(AsShared())](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
            {
                TSharedPtr<BCPortalOAuthClient> This = WeakThis.Pin();
                return This.IsValid() && This->HandleOAuthCallback(Request, OnComplete);
            }, 0));
    if (!RouteHandle.IsValid())
    {
        Fail(FString::Printf(TEXT("Could not register the OAuth redirect on port %d - is another Unreal Editor already logging in?"), ListenPort));
        return;
    }
    bAwaitingCallback = true;
    HttpServerModule.StartAllListeners();

    FPlatformProcess::LaunchURL(*AuthorizeUrl, nullptr, nullptr);
#endif
}

void BCPortalOAuthClient::CancelLogin()
{
#if BC_WIDGET_OAUTH_SUPPORTED
    StopListening();
#endif
}

bool BCPortalOAuthClient::HasAccessToken() const
{
    return Auth.IsValid() && !Auth->Account().accessToken.empty() &&
        Auth->Account().accessTokenExpiresAt > FDateTime::UtcNow().ToUnixTimestamp();
}

void BCPortalOAuthClient::RefreshApiKeyForTeam(const FString& TeamId, FBCApiKeyRefreshSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure)
{
    if (ServerUrl.IsEmpty() || !Auth.IsValid() || Auth->Account().accessToken.empty())
    {
        OnFailure.ExecuteIfBound(TEXT("No active login session - sign in again before switching teams."));
        return;
    }

    SendPortalRequest(braincloud::PortalAuth::ApiKeyRequest(ToNative(ServerUrl), Auth->Account().accessToken, ToNative(TeamId)),
        [OnSuccess, OnFailure](int32 StatusCode, const std::string& Body)
        {
            std::string ApiKey;
            std::string Error;
            if (braincloud::PortalAuth::ParseApiKey(StatusCode, Body, ApiKey, Error))
            {
                OnSuccess.ExecuteIfBound(FromNative(ApiKey));
            }
            else
            {
                OnFailure.ExecuteIfBound(FromNative(Error));
            }
        });
}

void BCPortalOAuthClient::ResumeLogin(const FString& InServerUrl, const FString& InAccessToken, int64 InExpiresAt,
    const FString& InPreferredTeamId, FBCLoginSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure)
{
    OnSuccessDelegate = OnSuccess;
    OnFailureDelegate = OnFailure;
    ServerUrl = InServerUrl;
    Auth = MakeUnique<braincloud::PortalAuth>();
    Auth->Resume(ToNative(ServerUrl), ToNative(InAccessToken), InExpiresAt, ToNative(InPreferredTeamId));
    SendNextRequest();
}

void BCPortalOAuthClient::SendNextRequest()
{
    if (!Auth.IsValid())
    {
        return;
    }

    if (Auth->IsComplete())
    {
        const braincloud::PortalAccount& Account = Auth->Account();
        FBCLoginResult Result;
        Result.Email = FromNative(Account.email);
        Result.AdminId = FromNative(Account.adminId);
        Result.ApiKey = FromNative(Account.apiKey);
        Result.CurrentTeamId = FromNative(Account.currentTeamId);
        Result.AccessToken = FromNative(Account.accessToken);
        Result.AccessTokenExpiresAt = Account.accessTokenExpiresAt;
        for (const braincloud::PortalTeam& NativeTeam : Account.teams)
        {
            FBCPortalTeam Team;
            Team.TeamId = FromNative(NativeTeam.teamId);
            Team.TeamName = FromNative(NativeTeam.teamName);
            Team.bApiEnabled = NativeTeam.apiEnabled;
            Result.Teams.Add(Team);
        }
        OnSuccessDelegate.ExecuteIfBound(Result);
        return;
    }

    braincloud::PortalRequest Request;
    if (!Auth->NextRequest(Request))
    {
        Fail(TEXT("The brainCloud portal login stopped unexpectedly."));
        return;
    }

    const braincloud::PortalAuth* Flow = Auth.Get();
    SendPortalRequest(Request, [WeakThis = TWeakPtr<BCPortalOAuthClient>(AsShared()), Flow](int32 StatusCode, const std::string& Body)
    {
        TSharedPtr<BCPortalOAuthClient> This = WeakThis.Pin();
        if (!This.IsValid() || This->Auth.Get() != Flow)
        {
            return;
        }
        std::string Error;
        if (!This->Auth->HandleResponse(StatusCode, Body, Error))
        {
            This->Fail(FromNative(Error));
            return;
        }
        This->SendNextRequest();
    });
}

void BCPortalOAuthClient::Fail(const FString& Message)
{
#if BC_WIDGET_OAUTH_SUPPORTED
    StopListening();
#endif
    OnFailureDelegate.ExecuteIfBound(Message);
}

#if BC_WIDGET_OAUTH_SUPPORTED

bool BCPortalOAuthClient::HandleOAuthCallback(const FHttpServerRequest& Request, const TFunction<void(TUniquePtr<FHttpServerResponse>&&)>& OnComplete)
{
    const FString Code = Request.QueryParams.FindRef(TEXT("code"));
    const FString State = Request.QueryParams.FindRef(TEXT("state"));
    UE_LOG(LogBCWidget, Log, TEXT("[OAuth] redirect received (%s)"), bAwaitingCallback ? TEXT("first") : TEXT("repeat, ignored"));

    const FString HtmlBody =
        TEXT("<html><body style=\"font-family:sans-serif;text-align:center;padding-top:4em;\">")
        TEXT("<h2>brainCloud sign-in complete</h2>")
        TEXT("<p>You may close this window and return to the Unreal Editor.</p>")
        TEXT("</body></html>");
    OnComplete(FHttpServerResponse::Create(HtmlBody, TEXT("text/html")));

    if (!bAwaitingCallback)
    {
        return true;
    }
    bAwaitingCallback = false;

    AsyncTask(ENamedThreads::GameThread, [WeakThis = TWeakPtr<BCPortalOAuthClient>(AsShared()), Code, State]()
    {
        TSharedPtr<BCPortalOAuthClient> This = WeakThis.Pin();
        if (This.IsValid())
        {
            This->CompleteCallback(Code, State);
        }
    });

    return true;
}

void BCPortalOAuthClient::CompleteCallback(const FString& Code, const FString& State)
{
    StopListeningAfterGrace();

    std::string Error;
    if (!Auth.IsValid() || !Auth->HandleRedirect(ToNative(Code), ToNative(State), Error))
    {
        Fail(Error.empty() ? FString(TEXT("The brainCloud portal login is no longer running.")) : FromNative(Error));
        return;
    }
    SendNextRequest();
}

void BCPortalOAuthClient::StopListeningAfterGrace()
{
    TWeakPtr<BCPortalOAuthClient> WeakSelf(AsShared());
    auto Tick = [WeakSelf](float)
    {
        if (TSharedPtr<BCPortalOAuthClient> This = WeakSelf.Pin())
        {
            This->StopListening();
        }
        return false;
    };
#if ENGINE_MAJOR_VERSION >= 5
    GraceTickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(Tick), 30.0f);
#else
    GraceTickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(Tick), 30.0f);
#endif
}

void BCPortalOAuthClient::StopListening()
{
    bAwaitingCallback = false;
    if (GraceTickerHandle.IsValid())
    {
#if ENGINE_MAJOR_VERSION >= 5
        FTSTicker::GetCoreTicker().RemoveTicker(GraceTickerHandle);
#else
        FTicker::GetCoreTicker().RemoveTicker(GraceTickerHandle);
#endif
        GraceTickerHandle.Reset();
    }

    if (Router.IsValid() && RouteHandle.IsValid())
    {
        Router->UnbindRoute(RouteHandle);
    }
    Router.Reset();
    RouteHandle.Reset();
}

#endif
