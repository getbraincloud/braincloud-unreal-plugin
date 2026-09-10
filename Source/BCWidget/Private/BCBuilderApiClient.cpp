// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCBuilderApiClient.h"
#include "BCWidgetPrivatePCH.h"
#include "Containers/StringConv.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonUtil.h"
#include "Misc/Base64.h"
#include "Runtime/Launch/Resources/Version.h"

const TCHAR* BCBuilderApiClient::ApiVersion = TEXT("v1");

void BCBuilderApiClient::Configure(const FString& InPortalUrl, const FString& InAdminEmail, const FString& InApiKey)
{
    PortalUrl = InPortalUrl;
    AdminEmail = InAdminEmail;
    ApiKey = InApiKey;
}

FString BCBuilderApiClient::GetApiHost() const
{
    // The portal's Builder API is served from the "api." host, not "portal." - same swap the
    // reference Unity plugin's BuilderAPI.cs performs.
    if (PortalUrl.Contains(TEXT("portal.")))
    {
        FString Result = PortalUrl;
        Result.ReplaceInline(TEXT("portal."), TEXT("api."));
        return Result;
    }
    return PortalUrl;
}

void BCBuilderApiClient::GetTeams(FBCApiResponseDelegate OnComplete)
{
    SendRequest(FString::Printf(TEXT("/builder/%s/team"), ApiVersion), TEXT("GET"), FString(), OnComplete);
}

void BCBuilderApiClient::GetApps(const FString& TeamId, FBCApiResponseDelegate OnComplete)
{
    SendRequest(FString::Printf(TEXT("/builder/%s/team/%s/app"), ApiVersion, *TeamId), TEXT("GET"), FString(), OnComplete);
}

void BCBuilderApiClient::GetAppSecret(const FString& TeamId, const FString& AppId, FBCApiResponseDelegate OnComplete)
{
    SendRequest(FString::Printf(TEXT("/builder/%s/team/%s/app/%s/appsecret"), ApiVersion, *TeamId, *AppId),
        TEXT("GET"), FString(), OnComplete);
}

void BCBuilderApiClient::CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
    const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled, FBCApiResponseDelegate OnComplete)
{
    TSharedRef<FJsonObject> AppOptions = MakeShared<FJsonObject>();
    TArray<TSharedPtr<FJsonValue>> PlatformValues;
    for (const FString& Platform : SupportedPlatforms)
    {
        PlatformValues.Add(MakeShared<FJsonValueString>(Platform));
    }
    AppOptions->SetArrayField(TEXT("supportedPlatforms"), PlatformValues);
    AppOptions->SetBoolField(TEXT("gamificationEnabled"), bGamificationEnabled);

    TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("appName"), AppName);
    if (!TemplateAppId.IsEmpty())
    {
        Body->SetStringField(TEXT("templateAppId"), TemplateAppId);
    }
    Body->SetObjectField(TEXT("appOptions"), AppOptions);

    FString BodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
    FJsonSerializer::Serialize(Body, Writer);

    SendRequest(FString::Printf(TEXT("/builder/%s/team/%s/app"), ApiVersion, *TeamId), TEXT("POST"), BodyString, OnComplete);
}

void BCBuilderApiClient::GetTemplateApps(FBCApiResponseDelegate OnComplete)
{
    // Hardcoded v1 - matches the reference Unity plugin, which does not key this one off config.Version either.
    SendRequest(TEXT("/builder/v1/utility/templateapps?liveOnly=true"), TEXT("GET"), FString(), OnComplete);
}

void BCBuilderApiClient::SendRequest(const FString& Path, const FString& Verb, const FString& Body, FBCApiResponseDelegate OnComplete)
{
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25) || ENGINE_MAJOR_VERSION == 5
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
#else
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
#endif

    Request->SetURL(GetApiHost() + Path);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

    FTCHARToUTF8 Utf8Credentials(*(AdminEmail + TEXT(":") + ApiKey));
    const FString Credentials = FBase64::Encode(reinterpret_cast<const uint8*>(Utf8Credentials.Get()), Utf8Credentials.Length());
    Request->SetHeader(TEXT("Authorization"), TEXT("Basic ") + Credentials);

    if (!Body.IsEmpty())
    {
        Request->SetContentAsString(Body);
    }

    const FString LoggedUrl = Request->GetURL();
    UE_LOG(LogBCWidget, Log, TEXT("[BuilderAPI] -> %s %s"), *Verb, *LoggedUrl);

    Request->OnProcessRequestComplete().BindLambda(
        [OnComplete, LoggedUrl](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
        {
            if (!bConnectedSuccessfully || !Response.IsValid())
            {
                UE_LOG(LogBCWidget, Error, TEXT("[BuilderAPI] <- connection failed: %s"), *LoggedUrl);
                OnComplete.ExecuteIfBound(false, nullptr, TEXT("Could not reach the brainCloud portal."));
                return;
            }

            const int32 StatusCode = Response->GetResponseCode();
            const FString RawContent = Response->GetContentAsString();
            const TSharedPtr<FJsonObject> Json = JsonUtil::jsonStringToValue(RawContent);

            if (StatusCode < 200 || StatusCode >= 300)
            {
                FString ErrorMessage = FString::Printf(TEXT("Portal returned HTTP %d"), StatusCode);
                FString ServerError;
                if (Json.IsValid() && (Json->TryGetStringField(TEXT("error"), ServerError) || Json->TryGetStringField(TEXT("errors"), ServerError)))
                {
                    ErrorMessage = ServerError;
                }
                UE_LOG(LogBCWidget, Warning, TEXT("[BuilderAPI] <- %d %s -- body: %s"), StatusCode, *LoggedUrl, *RawContent);
                OnComplete.ExecuteIfBound(false, Json, ErrorMessage);
                return;
            }

            UE_LOG(LogBCWidget, Log, TEXT("[BuilderAPI] <- %d %s -- body: %s"), StatusCode, *LoggedUrl, *RawContent);

            // Every Builder API response wraps its payload in a top-level "response" object
            // (confirmed against the reference Unity plugin for team/app/appsecret/create/template -
            // all five endpoints). Unwrap it once here so every call site gets the inner payload directly.
            const TSharedPtr<FJsonObject>* InnerResponse = nullptr;
            if (Json.IsValid() && Json->TryGetObjectField(TEXT("response"), InnerResponse) && InnerResponse)
            {
                OnComplete.ExecuteIfBound(true, *InnerResponse, FString());
            }
            else
            {
                OnComplete.ExecuteIfBound(true, Json, FString());
            }
        });

    Request->ProcessRequest();
}
