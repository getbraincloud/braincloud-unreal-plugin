// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCUtilityWidgetBase.h"
#include "BCEditorCredentialsStore.h"

namespace
{
    // Shared by the app list and template app list endpoints - both wrap their payload the
    // same way: { "apps": [ { "appName": ..., "appId": ... }, ... ] }.
    template <typename TAppStruct>
    TArray<TAppStruct> ParseAppArray(const TSharedPtr<FJsonObject>& Json)
    {
        TArray<TAppStruct> Result;
        if (!Json.IsValid())
        {
            return Result;
        }

        const TArray<TSharedPtr<FJsonValue>>* Apps = nullptr;
        if (Json->TryGetArrayField(TEXT("apps"), Apps) && Apps)
        {
            for (const TSharedPtr<FJsonValue>& AppValue : *Apps)
            {
                const TSharedPtr<FJsonObject>* AppObj;
                if (AppValue.IsValid() && AppValue->TryGetObject(AppObj))
                {
                    TAppStruct App;
                    (*AppObj)->TryGetStringField(TEXT("appId"), App.AppId);
                    (*AppObj)->TryGetStringField(TEXT("appName"), App.AppName);
                    Result.Add(App);
                }
            }
        }
        return Result;
    }
}

UBCUtilityWidgetBase::UBCUtilityWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    OAuthClient = MakeShared<BCPortalOAuthClient>();
    BuilderApiClient = MakeShared<BCBuilderApiClient>();
}

bool UBCUtilityWidgetBase::IsOAuthAvailable() const
{
    return BCPortalOAuthClient::IsOAuthSupported();
}

void UBCUtilityWidgetBase::StartLogin(const FString& ServerUrl, const FString& ClientId)
{
    CurrentServerUrl = ServerUrl;
    OAuthClient->StartLogin(ServerUrl, ClientId,
        FBCLoginSuccessDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleLoginSuccess),
        FBCLoginFailureDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleLoginFailure));
}

void UBCUtilityWidgetBase::CancelLogin()
{
    OAuthClient->CancelLogin();
}

void UBCUtilityWidgetBase::Logout()
{
    BCEditorCredentialsStore::Clear();
    CurrentAdminEmail.Empty();
    CurrentTeamId.Empty();
    CurrentApiKey.Empty();
}

bool UBCUtilityWidgetBase::TryRestoreRememberedAccount(const FString& ServerUrl, FString& OutAdminEmail, FString& OutTeamId)
{
    const FBCEditorCredentials Credentials = BCEditorCredentialsStore::Load();
    if (!Credentials.IsValid())
    {
        return false;
    }

    CurrentServerUrl = ServerUrl;
    CurrentAdminEmail = Credentials.AdminEmail;
    CurrentTeamId = Credentials.TeamId;
    CurrentApiKey = Credentials.ApiKey;
    BuilderApiClient->Configure(ServerUrl, CurrentAdminEmail, CurrentApiKey);

    OutAdminEmail = CurrentAdminEmail;
    OutTeamId = CurrentTeamId;
    return true;
}

void UBCUtilityWidgetBase::SwitchTeam(const FString& TeamId)
{
    PendingSwitchTeamId = TeamId;
    OAuthClient->RefreshApiKeyForTeam(TeamId,
        FBCApiKeyRefreshSuccessDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleApiKeyRefreshed),
        FBCLoginFailureDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleApiKeyRefreshFailed));
}

void UBCUtilityWidgetBase::HandleApiKeyRefreshed(const FString& NewApiKey)
{
    CurrentApiKey = NewApiKey;
    BuilderApiClient->Configure(CurrentServerUrl, CurrentAdminEmail, NewApiKey);

    FBCEditorCredentials Credentials;
    Credentials.AdminEmail = CurrentAdminEmail;
    Credentials.TeamId = PendingSwitchTeamId;
    Credentials.ApiKey = NewApiKey;
    BCEditorCredentialsStore::Save(Credentials);

    FetchApps(PendingSwitchTeamId);
}

void UBCUtilityWidgetBase::HandleApiKeyRefreshFailed(const FString& ErrorMessage)
{
    OnRequestFailed(ErrorMessage);
}

void UBCUtilityWidgetBase::FetchTeams()
{
    BuilderApiClient->GetTeams(FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleTeamsResponse));
}

void UBCUtilityWidgetBase::FetchApps(const FString& TeamId)
{
    CurrentTeamId = TeamId;
    BuilderApiClient->GetApps(TeamId, FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleAppsResponse));
}

void UBCUtilityWidgetBase::FetchAppSecret(const FString& TeamId, const FString& AppId)
{
    PendingAppSecretAppId = AppId;
    BuilderApiClient->GetAppSecret(TeamId, AppId, FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleAppSecretResponse));
}

void UBCUtilityWidgetBase::FetchTemplateApps()
{
    BuilderApiClient->GetTemplateApps(FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleTemplateAppsResponse));
}

void UBCUtilityWidgetBase::CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
    const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled)
{
    CurrentTeamId = TeamId;
    BuilderApiClient->CreateApp(TeamId, AppName, TemplateAppId, SupportedPlatforms, bGamificationEnabled,
        FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleCreateAppResponse));
}

void UBCUtilityWidgetBase::HandleLoginSuccess(const FBCLoginResult& Result)
{
    CurrentAdminEmail = Result.Email;
    CurrentTeamId = Result.CurrentTeamId;
    CurrentApiKey = Result.ApiKey;

    FBCEditorCredentials Credentials;
    Credentials.AdminEmail = Result.Email;
    Credentials.TeamId = Result.CurrentTeamId;
    Credentials.ApiKey = Result.ApiKey;
    BCEditorCredentialsStore::Save(Credentials);

    BuilderApiClient->Configure(CurrentServerUrl, Result.Email, Result.ApiKey);

    OnLoginSucceeded(Result.Email);
    OnTeamsReceived(Result.Teams);
}

void UBCUtilityWidgetBase::HandleLoginFailure(const FString& ErrorMessage)
{
    OnLoginFailed(ErrorMessage);
}

void UBCUtilityWidgetBase::HandleTeamsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        OnRequestFailed(ErrorMessage);
        return;
    }

    // NOTE: GET /builder/{v}/team is not exercised anywhere in the reference Unity plugin (its
    // team list comes entirely from the OAuth /user/me response instead), so this shape - a
    // "teams" array of {teamId, teamName, apiEnabled}, by analogy with the apps/templateapps
    // endpoints - is inferred, not confirmed against a live response. Verify before relying on
    // this in a "change team" UI for a resumed (non-OAuth) session.
    TArray<FBCPortalTeam> Teams;
    if (Json.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* TeamValues = nullptr;
        if (Json->TryGetArrayField(TEXT("teams"), TeamValues) && TeamValues)
        {
            for (const TSharedPtr<FJsonValue>& TeamValue : *TeamValues)
            {
                const TSharedPtr<FJsonObject>* TeamObj;
                if (TeamValue.IsValid() && TeamValue->TryGetObject(TeamObj))
                {
                    FBCPortalTeam Team;
                    (*TeamObj)->TryGetStringField(TEXT("teamId"), Team.TeamId);
                    (*TeamObj)->TryGetStringField(TEXT("teamName"), Team.TeamName);
                    (*TeamObj)->TryGetBoolField(TEXT("apiEnabled"), Team.bApiEnabled);
                    Teams.Add(Team);
                }
            }
        }
    }
    OnTeamsReceived(Teams);
}

void UBCUtilityWidgetBase::HandleAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        OnRequestFailed(ErrorMessage);
        return;
    }
    OnAppsReceived(ParseAppArray<FBCPortalApp>(Json));
}

void UBCUtilityWidgetBase::HandleAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        OnRequestFailed(ErrorMessage);
        return;
    }

    FString AppSecret;
    if (Json.IsValid())
    {
        const TSharedPtr<FJsonObject>* AppObj = nullptr;
        if (Json->TryGetObjectField(TEXT("app"), AppObj) && AppObj)
        {
            (*AppObj)->TryGetStringField(TEXT("appSecret"), AppSecret);
        }
    }
    OnAppSecretReceived(PendingAppSecretAppId, AppSecret);
}

void UBCUtilityWidgetBase::HandleTemplateAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        OnRequestFailed(ErrorMessage);
        return;
    }
    OnTemplateAppsReceived(ParseAppArray<FBCPortalTemplateApp>(Json));
}

void UBCUtilityWidgetBase::HandleCreateAppResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        OnRequestFailed(ErrorMessage);
        return;
    }

    FString AppId, AppName;
    if (Json.IsValid())
    {
        const TSharedPtr<FJsonObject>* AppObj = nullptr;
        if (Json->TryGetObjectField(TEXT("app"), AppObj) && AppObj)
        {
            (*AppObj)->TryGetStringField(TEXT("appId"), AppId);
            (*AppObj)->TryGetStringField(TEXT("appName"), AppName);
        }
    }

    if (AppId.IsEmpty())
    {
        OnRequestFailed(TEXT("Portal did not return an appId for the newly created app."));
        return;
    }

    PendingCreateAppId = AppId;
    PendingCreateAppName = AppName;

    // The create response doesn't reliably carry the app secret - fetch it explicitly so
    // OnAppCreated always hands back something usable for SetBCAppData.
    BuilderApiClient->GetAppSecret(CurrentTeamId, AppId,
        FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleCreateAppSecretResponse));
}

void UBCUtilityWidgetBase::HandleCreateAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        // The app was created even though the secret fetch failed - still tell the caller,
        // with an empty secret, rather than silently dropping the successful creation.
        OnAppCreated(PendingCreateAppId, PendingCreateAppName, FString());
        return;
    }

    FString AppSecret;
    if (Json.IsValid())
    {
        const TSharedPtr<FJsonObject>* AppObj = nullptr;
        if (Json->TryGetObjectField(TEXT("app"), AppObj) && AppObj)
        {
            (*AppObj)->TryGetStringField(TEXT("appSecret"), AppSecret);
        }
    }

    OnAppCreated(PendingCreateAppId, PendingCreateAppName, AppSecret);
}
