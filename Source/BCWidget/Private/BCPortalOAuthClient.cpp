// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCPortalOAuthClient.h"
#include "BCOAuthPkce.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HAL/PlatformProcess.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonUtil.h"
#include "Async/Async.h"
#include "Runtime/Launch/Resources/Version.h"

#if BC_WIDGET_OAUTH_SUPPORTED
#include "HttpPath.h"
#include "HttpServerModule.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "IHttpRouter.h"
#endif

namespace
{
    const TCHAR* GOAuthScope = TEXT("openid email builder_api team_info app_info team_read app_read app_create utility_read");
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
    ClientId = InClientId;
    RedirectUri = FString::Printf(TEXT("http://localhost:%d/oauth/callback"), ListenPort);
    CodeVerifier = BCOAuthPkce::GenerateCodeVerifier();
    ExpectedState = BCOAuthPkce::GenerateState();
    const FString CodeChallenge = BCOAuthPkce::GenerateCodeChallenge(CodeVerifier);
    PendingResult = FBCLoginResult();
    AccessToken.Empty();

    FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
    Router = HttpServerModule.GetHttpRouter(ListenPort, /*bFailOnBindFailure=*/false);
    if (!Router.IsValid())
    {
        Fail(FString::Printf(TEXT("Could not open the OAuth redirect listener on port %d - is another Unreal Editor already logging in?"), ListenPort));
        return;
    }

    RouteHandle = Router->BindRoute(FHttpPath(TEXT("/oauth/callback")), EHttpServerRequestVerbs::VERB_GET,
        FHttpRequestHandler::CreateLambda(
            [this](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
            {
                return HandleOAuthCallback(Request, OnComplete);
            }));
    HttpServerModule.StartAllListeners();

    const FString AuthorizeUrl = FString::Printf(
        TEXT("%s/oauth/authorize?response_type=code&client_id=%s&redirect_uri=%s&scope=%s&state=%s&code_challenge=%s&code_challenge_method=S256"),
        *ServerUrl,
        *FGenericPlatformHttp::UrlEncode(ClientId),
        *FGenericPlatformHttp::UrlEncode(RedirectUri),
        *FGenericPlatformHttp::UrlEncode(GOAuthScope),
        *FGenericPlatformHttp::UrlEncode(ExpectedState),
        *FGenericPlatformHttp::UrlEncode(CodeChallenge));

    FPlatformProcess::LaunchURL(*AuthorizeUrl, nullptr, nullptr);
#endif
}

void BCPortalOAuthClient::CancelLogin()
{
#if BC_WIDGET_OAUTH_SUPPORTED
    StopListening();
#endif
}

void BCPortalOAuthClient::RefreshApiKeyForTeam(const FString& TeamId, FBCApiKeyRefreshSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure)
{
    if (ServerUrl.IsEmpty() || AccessToken.IsEmpty())
    {
        OnFailure.ExecuteIfBound(TEXT("No active login session - sign in again before switching teams."));
        return;
    }

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif
    Request->SetURL(ServerUrl + TEXT("/user/add-temp-api-key?teamId=") + FGenericPlatformHttp::UrlEncode(TeamId));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AccessToken);
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetContentAsString(TEXT(""));

    Request->OnProcessRequestComplete().BindLambda(
        [OnSuccess, OnFailure](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
        {
            if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                OnFailure.ExecuteIfBound(TEXT("Could not enable the Builder API for this team - check that it's enabled in Team Setup on the portal."));
                return;
            }

            const TSharedPtr<FJsonObject> Json = JsonUtil::jsonStringToValue(Response->GetContentAsString());
            FString NewApiKey;
            const TSharedPtr<FJsonObject>* ApiKeyObj = nullptr;
            if (Json.IsValid() && Json->TryGetObjectField(TEXT("apiKey"), ApiKeyObj) && ApiKeyObj && (*ApiKeyObj)->Values.Num() > 0)
            {
                auto It = (*ApiKeyObj)->Values.CreateConstIterator();
                const TSharedPtr<FJsonObject>* InnerObj;
                if (It->Value.IsValid() && It->Value->TryGetObject(InnerObj))
                {
                    (*InnerObj)->TryGetStringField(TEXT("apiKey"), NewApiKey);
                }
            }

            if (NewApiKey.IsEmpty())
            {
                OnFailure.ExecuteIfBound(TEXT("Portal did not return a Builder API key for this team."));
                return;
            }

            OnSuccess.ExecuteIfBound(NewApiKey);
        });
    Request->ProcessRequest();
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

    const FString HtmlBody =
        TEXT("<html><body style=\"font-family:sans-serif;text-align:center;padding-top:4em;\">")
        TEXT("<h2>brainCloud sign-in complete</h2>")
        TEXT("<p>You may close this window and return to the Unreal Editor.</p>")
        TEXT("</body></html>");
    OnComplete(FHttpServerResponse::Create(HtmlBody, TEXT("text/html")));

    // Don't unbind the route / touch router state synchronously from inside its own dispatch -
    // defer to the next game thread tick.
    AsyncTask(ENamedThreads::GameThread, [this, Code, State]()
    {
        StopListening();

        if (State.IsEmpty() || State != ExpectedState)
        {
            Fail(TEXT("OAuth state mismatch - login was not initiated by this editor session."));
            return;
        }
        if (Code.IsEmpty())
        {
            Fail(TEXT("Login was cancelled or the portal did not return an authorization code."));
            return;
        }

        ExchangeToken(Code);
    });

    return true;
}

void BCPortalOAuthClient::StopListening()
{
    // Only unbind our own route - HTTPServer's Start/StopAllListeners is process-global, and
    // other systems in the same editor may be using it, so we deliberately never call
    // StopAllListeners() here. An idle router with no bound routes is harmless.
    if (Router.IsValid() && RouteHandle.IsValid())
    {
        Router->UnbindRoute(RouteHandle);
    }
    Router.Reset();
    RouteHandle.Reset();
}

#endif // BC_WIDGET_OAUTH_SUPPORTED

void BCPortalOAuthClient::ExchangeToken(const FString& AuthCode)
{
    const FString Body = FString::Printf(
        TEXT("client_id=%s&grant_type=authorization_code&redirect_uri=%s&code=%s&code_verifier=%s"),
        *FGenericPlatformHttp::UrlEncode(ClientId),
        *FGenericPlatformHttp::UrlEncode(RedirectUri),
        *FGenericPlatformHttp::UrlEncode(AuthCode),
        *FGenericPlatformHttp::UrlEncode(CodeVerifier));

#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif
    Request->SetURL(ServerUrl + TEXT("/oauth/token"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetContentAsString(Body);

    Request->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
        {
            if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                Fail(TEXT("Could not exchange the authorization code for an access token."));
                return;
            }

            const TSharedPtr<FJsonObject> Json = JsonUtil::jsonStringToValue(Response->GetContentAsString());
            FString Token;
            if (!Json.IsValid() || !Json->TryGetStringField(TEXT("access_token"), Token))
            {
                Fail(TEXT("Portal token response did not contain an access_token."));
                return;
            }

            AccessToken = Token;
            FetchUserInfo();
        });
    Request->ProcessRequest();
}

void BCPortalOAuthClient::FetchUserInfo()
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif
    Request->SetURL(ServerUrl + TEXT("/user/me"));
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AccessToken);
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

    Request->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
        {
            if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                Fail(TEXT("Could not fetch account info from the brainCloud portal."));
                return;
            }

            const TSharedPtr<FJsonObject> Json = JsonUtil::jsonStringToValue(Response->GetContentAsString());
            if (!Json.IsValid())
            {
                Fail(TEXT("Portal /user/me response was not valid JSON."));
                return;
            }

            Json->TryGetStringField(TEXT("email"), PendingResult.Email);
            if (!Json->TryGetStringField(TEXT("id"), PendingResult.AdminId))
            {
                Json->TryGetStringField(TEXT("sub"), PendingResult.AdminId);
            }

            const TSharedPtr<FJsonObject>* TeamInfo = nullptr;
            if (Json->TryGetObjectField(TEXT("team_info"), TeamInfo) && TeamInfo)
            {
                const TSharedPtr<FJsonObject>* CurrentTeam = nullptr;
                if ((*TeamInfo)->TryGetObjectField(TEXT("current_team"), CurrentTeam) && CurrentTeam)
                {
                    (*CurrentTeam)->TryGetStringField(TEXT("teamId"), PendingResult.CurrentTeamId);
                }

                const TArray<TSharedPtr<FJsonValue>>* AvailableTeams = nullptr;
                if ((*TeamInfo)->TryGetArrayField(TEXT("available_teams"), AvailableTeams) && AvailableTeams)
                {
                    for (const TSharedPtr<FJsonValue>& TeamValue : *AvailableTeams)
                    {
                        const TSharedPtr<FJsonObject>* TeamObj;
                        if (TeamValue.IsValid() && TeamValue->TryGetObject(TeamObj))
                        {
                            FBCPortalTeam Team;
                            (*TeamObj)->TryGetStringField(TEXT("teamId"), Team.TeamId);
                            (*TeamObj)->TryGetStringField(TEXT("teamName"), Team.TeamName);
                            (*TeamObj)->TryGetBoolField(TEXT("apiEnabled"), Team.bApiEnabled);
                            PendingResult.Teams.Add(Team);
                        }
                    }
                }
            }

            if (PendingResult.CurrentTeamId.IsEmpty() && PendingResult.Teams.Num() > 0)
            {
                PendingResult.CurrentTeamId = PendingResult.Teams[0].TeamId;
            }

            if (PendingResult.CurrentTeamId.IsEmpty())
            {
                Fail(TEXT("Your brainCloud account has no teams to select from."));
                return;
            }

            AddTempApiKey();
        });
    Request->ProcessRequest();
}

void BCPortalOAuthClient::AddTempApiKey()
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif
    Request->SetURL(ServerUrl + TEXT("/user/add-temp-api-key?teamId=") + FGenericPlatformHttp::UrlEncode(PendingResult.CurrentTeamId));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AccessToken);
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetContentAsString(TEXT(""));

    Request->OnProcessRequestComplete().BindLambda(
        [this](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
        {
            if (!bConnectedSuccessfully || !Response.IsValid() || Response->GetResponseCode() < 200 || Response->GetResponseCode() >= 300)
            {
                Fail(TEXT("Could not enable the Builder API for this team - check that it's enabled in Team Setup on the portal."));
                return;
            }

            const TSharedPtr<FJsonObject> Json = JsonUtil::jsonStringToValue(Response->GetContentAsString());
            const TSharedPtr<FJsonObject>* ApiKeyObj = nullptr;
            if (Json.IsValid() && Json->TryGetObjectField(TEXT("apiKey"), ApiKeyObj) && ApiKeyObj && (*ApiKeyObj)->Values.Num() > 0)
            {
                auto It = (*ApiKeyObj)->Values.CreateConstIterator();
                const TSharedPtr<FJsonObject>* InnerObj;
                if (It->Value.IsValid() && It->Value->TryGetObject(InnerObj))
                {
                    (*InnerObj)->TryGetStringField(TEXT("apiKey"), PendingResult.ApiKey);
                }
            }

            if (PendingResult.ApiKey.IsEmpty())
            {
                Fail(TEXT("Portal did not return a Builder API key for this team."));
                return;
            }

            OnSuccessDelegate.ExecuteIfBound(PendingResult);
        });
    Request->ProcessRequest();
}
