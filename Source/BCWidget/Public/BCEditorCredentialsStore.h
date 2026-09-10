// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Persists editor-only admin state (email, selected team, Builder API key) across editor
// restarts via Config/BCEditorSettings.ini, mirroring BrainCloudFunctionLibrary::GetBCAppData/
// SetBCAppData's GConfig read/write shape. Deliberately does NOT persist the OAuth access
// token (short-lived) - a restart requires re-login, same as the reference Unity plugin's
// per-session access token.
//
// Values are lightly bit-rotated before writing, purely to avoid plaintext grep-ability in the
// ini file - this is cosmetic, not encryption (the reference Unity plugin's own obfuscation
// carries the same caveat). Callers are responsible for telling consumers this file should not
// be committed to source control (see README).
struct FBCEditorCredentials
{
    FString AdminEmail;
    FString TeamId;
    FString ApiKey;

    bool IsValid() const { return !AdminEmail.IsEmpty() && !ApiKey.IsEmpty(); }
};

class BCEditorCredentialsStore
{
public:
    static FBCEditorCredentials Load();
    static void Save(const FBCEditorCredentials& Credentials);
    static void Clear();

private:
    static FString Obfuscate(const FString& PlainText);
    static FString Deobfuscate(const FString& Obfuscated);
};
