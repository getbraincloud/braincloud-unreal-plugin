// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCPortalTypes.h"

#if BC_WIDGET_OAUTH_SUPPORTED
#include "HttpRouteHandle.h"
class IHttpRouter;
struct FHttpServerRequest;
#endif

// Result of a completed portal login: identity + team list + the durable Builder API key
// (from add-temp-api-key) used for all subsequent Basic-auth Builder API calls.
struct FBCLoginResult
{
    FString Email;
    FString AdminId;
    FString ApiKey;
    FString CurrentTeamId;
    TArray<FBCPortalTeam> Teams;
};

DECLARE_DELEGATE_OneParam(FBCLoginSuccessDelegate, const FBCLoginResult&);
DECLARE_DELEGATE_OneParam(FBCLoginFailureDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FBCApiKeyRefreshSuccessDelegate, const FString& /*NewApiKey*/);

// Drives the portal OAuth Authorization Code + PKCE flow: opens the system browser to
// {ServerUrl}/oauth/authorize, catches the redirect on a local loopback listener, exchanges
// the code for an access token, then mints a durable Builder API key via add-temp-api-key.
// Mirrors the reference Unity plugin's PluginEditor.LoginOAuth() / OAuthRedirectServer.cs.
//
// Below UE 4.24 (HTTPServer module, checked via IsOAuthSupported()) the loopback listener
// isn't available - the caller should hide/disable the Login affordance in that case rather
// than call StartLogin().
class BCPortalOAuthClient
{
public:
    ~BCPortalOAuthClient();

    static bool IsOAuthSupported() { return BC_WIDGET_OAUTH_SUPPORTED != 0; }

    // ServerUrl: the portal base URL (e.g. https://portal.braincloudservers.com).
    // ClientId: must be a client_id registered with the brainCloud portal's OAuth allow-list
    // for this plugin's redirect URI ahead of time - this is an external portal-side prerequisite,
    // not something this client can self-serve.
    void StartLogin(const FString& ServerUrl, const FString& ClientId,
        FBCLoginSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure);

    // Matches the reference plugin's manual Cancel button (there is no auto-timeout - the
    // loopback listener just waits, it doesn't block anything, until the user cancels or completes).
    void CancelLogin();

    // Re-mints the Builder API key for a specific team, reusing the access token from the
    // most recent login. Each add-temp-api-key grant is scoped to ONE team - it is not a
    // global grant across every team the admin belongs to - so switching teams requires a
    // fresh key for the newly-selected one. Matches the reference Unity plugin's
    // GetInnerAPIKey(), which SettingsView.cs re-calls every time the team dropdown changes.
    // Only valid for the lifetime of the current login session (needs a live, still-fresh
    // AccessToken) - fails immediately if no login has completed yet this session.
    void RefreshApiKeyForTeam(const FString& TeamId, FBCApiKeyRefreshSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure);

private:
#if BC_WIDGET_OAUTH_SUPPORTED
    bool HandleOAuthCallback(const FHttpServerRequest& Request, const TFunction<void(TUniquePtr<struct FHttpServerResponse>&&)>& OnComplete);
    void StopListening();

    TSharedPtr<IHttpRouter> Router;
    FHttpRouteHandle RouteHandle;
#endif

    void ExchangeToken(const FString& AuthCode);
    void FetchUserInfo();
    void AddTempApiKey();
    void Fail(const FString& Message);

    FString ServerUrl;
    FString ClientId;
    FString RedirectUri;
    FString CodeVerifier;
    FString ExpectedState;
    FString AccessToken;

    FBCLoginResult PendingResult;

    FBCLoginSuccessDelegate OnSuccessDelegate;
    FBCLoginFailureDelegate OnFailureDelegate;

    // Fixed to match the reference Unity plugin's redirect_uri. Known limitation: two Unreal
    // Editors mid-login on the same machine at once will collide on this port - the bind failure
    // surfaces as a clean error (Fail()) rather than a crash.
    static constexpr int32 ListenPort = 45678;
};
