// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCPortalTypes.h"

#if BC_WIDGET_OAUTH_SUPPORTED
#include "HttpRouteHandle.h"
#include "Containers/Ticker.h"
#include "Runtime/Launch/Resources/Version.h"
class IHttpRouter;
struct FHttpServerRequest;
#endif

namespace braincloud
{
    class PortalAuth;
}

/**
 * The result of a login to the brainCloud portal.
 */
struct FBCLoginResult
{
    FString Email;
    FString AdminId;
    FString ApiKey;
    FString CurrentTeamId;
    FString AccessToken;
    int64 AccessTokenExpiresAt = 0;
    TArray<FBCPortalTeam> Teams;
};

DECLARE_DELEGATE_OneParam(FBCLoginSuccessDelegate, const FBCLoginResult&);
DECLARE_DELEGATE_OneParam(FBCLoginFailureDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FBCApiKeyRefreshSuccessDelegate, const FString&);

/**
 * Logs in to the brainCloud portal through the browser.
 */
class BCPortalOAuthClient : public TSharedFromThis<BCPortalOAuthClient>
{
public:
    BCPortalOAuthClient();
    ~BCPortalOAuthClient();

    static bool IsOAuthSupported() { return BC_WIDGET_OAUTH_SUPPORTED != 0; }

    void StartLogin(const FString& ServerUrl, const FString& ClientId,
        FBCLoginSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure);

    void CancelLogin();

    bool HasAccessToken() const;

    void ResumeLogin(const FString& InServerUrl, const FString& InAccessToken, int64 InExpiresAt,
        const FString& PreferredTeamId, FBCLoginSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure);

    void RefreshApiKeyForTeam(const FString& TeamId, FBCApiKeyRefreshSuccessDelegate OnSuccess, FBCLoginFailureDelegate OnFailure);

private:
#if BC_WIDGET_OAUTH_SUPPORTED
    bool HandleOAuthCallback(const FHttpServerRequest& Request, const TFunction<void(TUniquePtr<struct FHttpServerResponse>&&)>& OnComplete);
    void CompleteCallback(const FString& Code, const FString& State);
    void StopListening();
    void StopListeningAfterGrace();

    TSharedPtr<IHttpRouter> Router;
    FHttpRouteHandle RouteHandle;
    bool bAwaitingCallback = false;
#if ENGINE_MAJOR_VERSION >= 5
    FTSTicker::FDelegateHandle GraceTickerHandle;
#else
    FDelegateHandle GraceTickerHandle;
#endif
#endif

    void SendNextRequest();
    void Fail(const FString& Message);

    FString ServerUrl;
    TUniquePtr<braincloud::PortalAuth> Auth;

    FBCLoginSuccessDelegate OnSuccessDelegate;
    FBCLoginFailureDelegate OnFailureDelegate;
};
