// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * The brainCloud portal account remembered by the editor.
 */
struct FBCEditorCredentials
{
    FString AdminEmail;
    FString TeamId;
    FString ApiKey;
    FString AccessToken;
    int64 AccessTokenExpiresAt = 0;

    bool IsValid() const { return !AdminEmail.IsEmpty() && !ApiKey.IsEmpty(); }

    bool HasLiveAccessToken() const
    {
        return !AccessToken.IsEmpty() && AccessTokenExpiresAt > FDateTime::UtcNow().ToUnixTimestamp();
    }
};

/**
 * Saves the brainCloud portal account in the project Saved folder.
 */
class BCEditorCredentialsStore
{
public:
    static FBCEditorCredentials Load();
    static void Save(const FBCEditorCredentials& Credentials);
    static void Clear();
};
