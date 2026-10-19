// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCEditorCredentialsStore.h"
#include "BCEditorConfigStore.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

THIRD_PARTY_INCLUDES_START
#include "braincloud/BrainCloudNativeWriter.h"
THIRD_PARTY_INCLUDES_END

namespace
{
    FString GetSessionPath()
    {
        return FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("BrainCloud"), TEXT("EditorSession.ini")));
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

    braincloud::PortalSession Session;
    braincloud::ConfigWriter::ResolveSession(FBCEditorConfigStore::ToNative(GetSessionPath()), Session);

    FBCEditorCredentials Result;
    Result.AdminEmail = FBCEditorConfigStore::FromNative(Session.email);
    Result.TeamId = FBCEditorConfigStore::FromNative(Session.teamId);
    Result.ApiKey = FBCEditorConfigStore::FromNative(Session.apiKey);
    Result.AccessToken = FBCEditorConfigStore::FromNative(Session.accessToken);
    Result.AccessTokenExpiresAt = Session.expiresAtUnix;
    return Result;
}

void BCEditorCredentialsStore::Save(const FBCEditorCredentials& Credentials)
{
    DeleteLegacyFile();

    const FString SessionPath = GetSessionPath();
    IFileManager::Get().MakeDirectory(*FPaths::GetPath(SessionPath), true);

    braincloud::PortalSession Session;
    Session.email = FBCEditorConfigStore::ToNative(Credentials.AdminEmail);
    Session.teamId = FBCEditorConfigStore::ToNative(Credentials.TeamId);
    Session.apiKey = FBCEditorConfigStore::ToNative(Credentials.ApiKey);
    Session.accessToken = FBCEditorConfigStore::ToNative(Credentials.AccessToken);
    Session.expiresAtUnix = Credentials.AccessTokenExpiresAt;
    braincloud::ConfigWriter::SaveSession(FBCEditorConfigStore::ToNative(SessionPath), Session);
}

void BCEditorCredentialsStore::Clear()
{
    DeleteLegacyFile();
    IFileManager::Get().Delete(*GetSessionPath(), false, false, true);
}
