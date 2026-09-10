// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BCPortalTypes.h"
#include "BCPortalOAuthClient.h"
#include "BCBuilderApiClient.h"
#include "BCUtilityWidgetBase.generated.h"

// C++ base class for Content/EditorUtility/BCUtilityWidget(_UE4).uasset. Owns the OAuth +
// Builder API logic (reviewable, testable C++); the Blueprint graph owns layout only - it
// calls the BlueprintCallable entry points below and binds the BlueprintImplementableEvents
// to update its own UI. Mirrors the reference Unity plugin's PluginEditor.cs responsibilities.
UCLASS(Blueprintable)
class UBCUtilityWidgetBase : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    UBCUtilityWidgetBase(const FObjectInitializer& ObjectInitializer);

    // False below UE 4.24 (no HTTPServer module for the OAuth loopback listener) - the
    // Blueprint UI should hide/disable the Login button in that case.
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    bool IsOAuthAvailable() const;

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void StartLogin(const FString& ServerUrl, const FString& ClientId);

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void CancelLogin();

    // Clears the persisted admin session (Config/BCEditorSettings.ini) and this widget's
    // in-memory state. Does not revoke the token/key on the portal itself.
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void Logout();

    // Attempts to resume a previous session from Config/BCEditorSettings.ini without a fresh
    // OAuth round-trip (the persisted Builder API key is durable, unlike the OAuth access
    // token). Returns false if nothing is remembered; the Blueprint should show the Login view
    // in that case. A resumed session can still fail on its first real API call (e.g. a
    // revoked key) - that surfaces through OnRequestFailed like any other call.
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    bool TryRestoreRememberedAccount(const FString& ServerUrl, FString& OutAdminEmail, FString& OutTeamId);

    // The team the portal considers "current" for this account - set right before
    // OnTeamsReceived fires after a fresh login, so it's safe to call from within that
    // event's handler to pre-select a default entry in a team dropdown.
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    FString GetCurrentTeamId() const { return CurrentTeamId; }

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchTeams();

    // Use this (not FetchApps directly) whenever the user picks a different team - the
    // Builder API key from login/a prior team is scoped to ONE team, so switching teams
    // needs a fresh key minted for the new one first. Re-mints via the still-live OAuth
    // access token, reconfigures BuilderApiClient and persists the new key, then calls
    // FetchApps. Fails through OnRequestFailed if called without an active login session
    // (e.g. after TryRestoreRememberedAccount, which has no live access token to re-mint with).
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void SwitchTeam(const FString& TeamId);

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchApps(const FString& TeamId);

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchAppSecret(const FString& TeamId, const FString& AppId);

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchTemplateApps();

    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
        const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnLoginSucceeded(const FString& Email);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnLoginFailed(const FString& ErrorMessage);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnTeamsReceived(const TArray<FBCPortalTeam>& Teams);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppsReceived(const TArray<FBCPortalApp>& Apps);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppSecretReceived(const FString& AppId, const FString& AppSecret);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnTemplateAppsReceived(const TArray<FBCPortalTemplateApp>& TemplateApps);

    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppCreated(const FString& AppId, const FString& AppName, const FString& AppSecret);

    // Fired for any Builder API call failure (team/app/secret/create/template fetch) - a single
    // catch-all, mirroring the reference plugin's one top-level error path that resets to a
    // known-good view rather than leaving the UI in a stuck in-flight state.
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnRequestFailed(const FString& ErrorMessage);

private:
    void HandleLoginSuccess(const FBCLoginResult& Result);
    void HandleLoginFailure(const FString& ErrorMessage);

    void HandleApiKeyRefreshed(const FString& NewApiKey);
    void HandleApiKeyRefreshFailed(const FString& ErrorMessage);

    void HandleTeamsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleTemplateAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleCreateAppResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    // Chained after a successful CreateApp - the create response doesn't reliably include the
    // secret (the reference Unity plugin never reads one back from it), so a follow-up
    // GetAppSecret call fills it in before OnAppCreated fires.
    void HandleCreateAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);

    TSharedPtr<BCPortalOAuthClient> OAuthClient;
    TSharedPtr<BCBuilderApiClient> BuilderApiClient;

    FString CurrentServerUrl;
    FString CurrentAdminEmail;
    FString CurrentTeamId;
    FString CurrentApiKey;

    // Extra context for in-flight requests whose Blueprint-facing event needs a field
    // (AppId/AppName) that the raw HTTP response doesn't echo back.
    FString PendingAppSecretAppId;
    FString PendingSwitchTeamId;
    FString PendingCreateAppId;
    FString PendingCreateAppName;
};
