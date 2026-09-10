// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

// bSuccess, ResponseJson (may be null on transport failure), ErrorMessage (empty on success)
DECLARE_DELEGATE_ThreeParams(FBCApiResponseDelegate, bool, TSharedPtr<FJsonObject>, FString);

// Thin wrapper over the brainCloud portal's Builder API (team/app browsing + app creation).
// Mirrors the reference Unity plugin's BuilderAPI.cs - Basic auth (AdminEmail:ApiKey), the
// api./portal. host swap, and one method per endpoint.
class BCBuilderApiClient
{
public:
    void Configure(const FString& InPortalUrl, const FString& InAdminEmail, const FString& InApiKey);

    void GetTeams(FBCApiResponseDelegate OnComplete);
    void GetApps(const FString& TeamId, FBCApiResponseDelegate OnComplete);
    void GetAppSecret(const FString& TeamId, const FString& AppId, FBCApiResponseDelegate OnComplete);
    void CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
        const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled, FBCApiResponseDelegate OnComplete);
    void GetTemplateApps(FBCApiResponseDelegate OnComplete);

private:
    void SendRequest(const FString& Path, const FString& Verb, const FString& Body, FBCApiResponseDelegate OnComplete);
    FString GetApiHost() const;

    FString PortalUrl;
    FString AdminEmail;
    FString ApiKey;

    static const TCHAR* ApiVersion;
};
