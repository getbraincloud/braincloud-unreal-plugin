// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// PKCE (RFC 7636) helpers for the portal OAuth Authorization Code flow.
//
// Unreal's Core module ships FMD5/FSHA1 (Misc/SecureHash.h) but no SHA-256 - a portable
// SHA-256 needs either the optional PlatformCrypto plugin (not guaranteed enabled in a
// consuming project) or a vendored implementation. To keep this plugin's wide UE4/UE5
// support without forcing a plugin dependency, Sha256 below is a small self-contained
// implementation (public-domain algorithm, no external deps).
class BCOAuthPkce
{
public:
    // 64 hex characters (two concatenated GUIDs), matching the reference portal client's verifier shape.
    static FString GenerateCodeVerifier();

    // base64url(SHA256(ASCII bytes of Verifier)), no padding.
    static FString GenerateCodeChallenge(const FString& Verifier);

    static FString GenerateState();

private:
    static void Sha256(const uint8* Data, int32 Length, uint8 OutHash[32]);
    static FString Base64UrlEncode(const uint8* Data, int32 Length);
};
