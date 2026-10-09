// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Generates the values used to log in to the brainCloud portal.
 */
class BCOAuthPkce
{
public:
    static FString GenerateCodeVerifier();

    static FString GenerateCodeChallenge(const FString& Verifier);

    static FString GenerateState();

private:
    static void Sha256(const uint8* Data, int32 Length, uint8 OutHash[32]);
    static FString Base64UrlEncode(const uint8* Data, int32 Length);
};
