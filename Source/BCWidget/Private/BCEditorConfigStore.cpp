// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCEditorConfigStore.h"

#include "Containers/StringConv.h"
#include "HAL/FileManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"

THIRD_PARTY_INCLUDES_START
#include "braincloud/BrainCloudNativeWriter.h"
THIRD_PARTY_INCLUDES_END

namespace
{
    const TCHAR* GCredentialsSection = TEXT("Credentials");

    std::string NativeConfigPath()
    {
        return FBCEditorConfigStore::ToNative(FPaths::ConvertRelativePathToFull(FBCConfigStore::GetConfigPath()));
    }

    braincloud::AppCredentials ToNativeApp(const FBCEditorAppCredentials& App)
    {
        braincloud::AppCredentials Native;
        Native.appId = FBCEditorConfigStore::ToNative(App.AppId);
        Native.appName = FBCEditorConfigStore::ToNative(App.AppName);
        Native.secret = FBCEditorConfigStore::ToNative(App.AppSecret);
        return Native;
    }

    void Wipe(std::string& Value)
    {
        std::fill(Value.begin(), Value.end(), '\0');
    }

    // Removes plain text secrets and keys written by pre-release builds of the editor panel.
    void RemoveOldSettings(bool bRemoveSecrets)
    {
        const FString SettingsPath = FBCConfigStore::GetSettingsPath();
        if (!FPaths::FileExists(SettingsPath))
        {
            return;
        }
        GConfig->LoadFile(SettingsPath);

        TArray<FString> Keys = { TEXT("SecureVersion"), TEXT("AppName"), TEXT("ChildAppName"), TEXT("ChildAppCount") };
        if (bRemoveSecrets)
        {
            Keys.Add(TEXT("AppSecret"));
            Keys.Add(TEXT("ChildAppSecret"));
        }
        FString CountValue;
        GConfig->GetString(GCredentialsSection, TEXT("ChildAppCount"), CountValue, SettingsPath);
        const int32 Count = FCString::Atoi(*CountValue);
        TArray<FString> Sections = { TEXT("ClientTuning"), TEXT("ChildTuning") };
        for (int32 i = 0; i < Count; ++i)
        {
            Keys.Add(FString::Printf(TEXT("ChildAppId%d"), i));
            Keys.Add(FString::Printf(TEXT("ChildAppName%d"), i));
            Keys.Add(FString::Printf(TEXT("ChildAppSecret%d"), i));
            Sections.Add(FString::Printf(TEXT("ChildTuning%d"), i));
        }

        for (const FString& Key : Keys)
        {
            GConfig->RemoveKey(GCredentialsSection, *Key, SettingsPath);
        }
        for (const FString& Section : Sections)
        {
            if (GConfig->DoesSectionExist(*Section, SettingsPath))
            {
                GConfig->EmptySection(*Section, SettingsPath);
            }
        }
        GConfig->Flush(false, SettingsPath);
    }
}

std::string FBCEditorConfigStore::ToNative(const FString& Value)
{
    FTCHARToUTF8 Converted(*Value);
    return std::string(Converted.Get(), Converted.Length());
}

FString FBCEditorConfigStore::FromNative(const std::string& Value)
{
    return FString(UTF8_TO_TCHAR(Value.c_str()));
}

bool FBCEditorConfigStore::Save(const FBCEditorAppCredentials& App, const TArray<FBCEditorAppCredentials>& ChildApps,
    const FString& ServerUrl, FString& OutError)
{
    if (App.AppId.IsEmpty())
    {
        OutError = TEXT("Pick an app first.");
        return false;
    }

    braincloud::AppCredentials NativeApp = ToNativeApp(App);
    std::vector<braincloud::AppCredentials> NativeChildApps;
    for (const FBCEditorAppCredentials& Child : ChildApps)
    {
        NativeChildApps.push_back(ToNativeApp(Child));
    }

    const bool bSaved = braincloud::ConfigWriter::WriteConfig(NativeConfigPath(), NativeApp, NativeChildApps, ToNative(ServerUrl), std::string());

    Wipe(NativeApp.secret);
    for (braincloud::AppCredentials& Child : NativeChildApps)
    {
        Wipe(Child.secret);
    }

    if (!bSaved || FBCConfigStore::DetectState() != EBCConfigState::Ready)
    {
        OutError = FString::Printf(TEXT("Could not save the app configuration to %s. Pick the app again to fetch its secret."), *FBCConfigStore::GetConfigPath());
        return false;
    }

    RemoveOldSettings(true);
    UE_LOG(LogTemp, Log, TEXT("brainCloud app %s saved to %s"), *App.AppId, *FBCConfigStore::GetConfigPath());
    return true;
}

bool FBCEditorConfigStore::SaveServerUrl(const FString& ServerUrl)
{
    return FPaths::FileExists(FBCConfigStore::GetConfigPath()) &&
        braincloud::ConfigWriter::WriteSettings(NativeConfigPath(), ToNative(ServerUrl), std::string());
}

bool FBCEditorConfigStore::Resolve(FBCStoredCredentials& Out)
{
    if (!FBCConfigStore::Resolve(Out))
    {
        return false;
    }

    braincloud::AppName App;
    std::vector<braincloud::AppName> Children;
    if (FPaths::FileExists(FBCConfigStore::GetConfigPath()) && braincloud::ConfigWriter::ReadAppNames(NativeConfigPath(), App, Children))
    {
        Out.AppName = FromNative(App.appName);
        for (const braincloud::AppName& Child : Children)
        {
            if (FBCStoredChildApp* Stored = Out.ChildApps.FindByPredicate([&Child](const FBCStoredChildApp& C) { return C.AppId == FromNative(Child.appId); }))
            {
                Stored->AppName = FromNative(Child.appName);
            }
        }
    }
    return true;
}

FString FBCEditorConfigStore::ReadSecret(const FString& AppId)
{
    std::string Secret;
    if (!FPaths::FileExists(FBCConfigStore::GetConfigPath()) ||
        !braincloud::ConfigWriter::ReadSecret(NativeConfigPath(), ToNative(AppId), Secret))
    {
        return FString();
    }
    const FString Result = FromNative(Secret);
    Wipe(Secret);
    return Result;
}

bool FBCEditorConfigStore::MigrateLegacy(FString& OutError)
{
    if (FBCConfigStore::DetectState() != EBCConfigState::Legacy)
    {
        OutError = TEXT("No plain text credentials to migrate.");
        return false;
    }

    const FString SettingsPath = FBCConfigStore::GetSettingsPath();
    GConfig->LoadFile(SettingsPath);
    auto Read = [&SettingsPath](const TCHAR* Key)
    {
        FString Value;
        GConfig->GetString(GCredentialsSection, Key, Value, SettingsPath);
        return Value;
    };

    FBCEditorAppCredentials App;
    App.AppId = Read(TEXT("AppId"));
    App.AppSecret = Read(TEXT("AppSecret"));

    TArray<FBCEditorAppCredentials> ChildApps;
    FBCEditorAppCredentials Child;
    Child.AppId = Read(TEXT("ChildAppId"));
    Child.AppSecret = Read(TEXT("ChildAppSecret"));
    if (!Child.AppId.IsEmpty() && !Child.AppSecret.IsEmpty())
    {
        ChildApps.Add(Child);
    }

    if (!Save(App, ChildApps, Read(TEXT("ServerUrl")), OutError))
    {
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Moved the plain text brainCloud credentials for app %s to %s."), *App.AppId, *FBCConfigStore::GetConfigPath());
    return true;
}

void FBCEditorConfigStore::Clear()
{
    IFileManager::Get().Delete(*FBCConfigStore::GetConfigPath(), false, false, true);
    RemoveOldSettings(true);
}
