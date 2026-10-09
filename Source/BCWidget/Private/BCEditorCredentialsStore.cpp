// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCEditorCredentialsStore.h"
#include "BCSecureStore.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace
{
    FString GetSessionPath()
    {
        return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("BrainCloud"), TEXT("EditorSession.ini"));
    }

    FString GetLegacyPath()
    {
        return FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("BCEditorSettings.ini"));
    }

    void DeleteLegacyFile()
    {
        const FString Legacy = GetLegacyPath();
        if (FPaths::FileExists(Legacy))
        {
            IFileManager::Get().Delete(*Legacy);
        }
    }
}

FBCEditorCredentials BCEditorCredentialsStore::Load()
{
    DeleteLegacyFile();

    FBCEditorCredentials Result;
    TArray<FString> Lines;
    if (!FFileHelper::LoadFileToStringArray(Lines, *GetSessionPath()))
    {
        return Result;
    }

    for (const FString& Line : Lines)
    {
        FString Key, Value;
        if (!Line.Split(TEXT("="), &Key, &Value))
        {
            continue;
        }
        const FString Decoded = FBCSecureStore::DecodeValue(Value);
        if (Key == TEXT("AdminEmail")) Result.AdminEmail = Decoded;
        else if (Key == TEXT("TeamId")) Result.TeamId = Decoded;
        else if (Key == TEXT("ApiKey")) Result.ApiKey = Decoded;
        else if (Key == TEXT("AccessToken")) Result.AccessToken = Decoded;
        else if (Key == TEXT("ExpiresAt")) LexFromString(Result.AccessTokenExpiresAt, *Decoded);
    }
    return Result;
}

void BCEditorCredentialsStore::Save(const FBCEditorCredentials& Credentials)
{
    DeleteLegacyFile();

    auto Line = [](const TCHAR* Key, const FString& Value)
    {
        return FString::Printf(TEXT("%s=%s\n"), Key, *FBCSecureStore::EncodeValue(Value));
    };

    FString Contents = TEXT("[Session]\n");
    Contents += Line(TEXT("AdminEmail"), Credentials.AdminEmail);
    Contents += Line(TEXT("TeamId"), Credentials.TeamId);
    Contents += Line(TEXT("ApiKey"), Credentials.ApiKey);
    Contents += Line(TEXT("AccessToken"), Credentials.AccessToken);
    Contents += Line(TEXT("ExpiresAt"), LexToString(Credentials.AccessTokenExpiresAt));

    FFileHelper::SaveStringToFile(Contents, *GetSessionPath());
}

void BCEditorCredentialsStore::Clear()
{
    DeleteLegacyFile();
    IFileManager::Get().Delete(*GetSessionPath(), false, false, true);
}
