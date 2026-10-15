// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCConfigStore.h"

#include "BCClientPluginPrivatePCH.h"
#include "Containers/StringConv.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"

#if BC_NATIVE_CONFIG
THIRD_PARTY_INCLUDES_START
#include "braincloud/BrainCloudNative.h"
THIRD_PARTY_INCLUDES_END
#endif

namespace
{
	const TCHAR* GConfigFilename = TEXT("BrainCloudNative.ini");
	const TCHAR* GSettingsFilename = TEXT("BrainCloudSettings.ini");

	const TCHAR* GCredentialsSection = TEXT("Credentials");
	const TCHAR* GAppIdKey = TEXT("AppId");
	const TCHAR* GAppSecretKey = TEXT("AppSecret");
	const TCHAR* GServerUrlKey = TEXT("ServerUrl");
	const TCHAR* GVersionKey = TEXT("Version");
	const TCHAR* GChildAppIdKey = TEXT("ChildAppId");
	const TCHAR* GChildAppSecretKey = TEXT("ChildAppSecret");
	const TCHAR* GDebugLoggingKey = TEXT("DebugLogging");

	// Written by pre-release builds of the editor panel; such projects need their app set again.
	const TCHAR* GUnsupportedVersionKey = TEXT("SecureVersion");

	FString ReadSetting(const FString& Key)
	{
		const FString SettingsPath = FBCConfigStore::GetSettingsPath();
		if (!FPaths::FileExists(SettingsPath))
		{
			return FString();
		}
		GConfig->LoadFile(SettingsPath);
		FString Value;
		GConfig->GetString(GCredentialsSection, *Key, Value, SettingsPath);
		return Value;
	}

#if BC_NATIVE_CONFIG
	std::string ToStd(const FString& Value)
	{
		FTCHARToUTF8 Converted(*Value);
		return std::string(Converted.Get(), Converted.Length());
	}

	FString FromStd(const std::string& Value)
	{
		return FString(UTF8_TO_TCHAR(Value.c_str()));
	}

	TSharedPtr<braincloud::NativeConfig> LoadNativeConfig(const FString& Contents)
	{
		TSharedPtr<braincloud::NativeConfig> Config = MakeShared<braincloud::NativeConfig>();
		return Config->ResolveContents(ToStd(Contents)) ? Config : nullptr;
	}

	void ReadNativeConfig(const braincloud::NativeConfig& Config, FBCStoredCredentials& Out)
	{
		Out = FBCStoredCredentials();
		Out.AppId = FromStd(Config.GetAppId());
		Out.ServerUrl = FromStd(Config.GetServerUrl());
		Out.Version = FromStd(Config.GetAppVersion());
		for (const std::string& ChildAppId : Config.GetChildAppIds())
		{
			FBCStoredChildApp Child;
			Child.AppId = FromStd(ChildAppId);
			Out.ChildApps.Add(Child);
		}
	}
#endif
}

bool FBCConfigStore::IsSupported()
{
	return BC_NATIVE_CONFIG != 0;
}

FString FBCConfigStore::GetConfigPath()
{
	return FPaths::ProjectConfigDir() + GConfigFilename;
}

FString FBCConfigStore::GetSettingsPath()
{
	FString SettingsPath = FPaths::ProjectConfigDir() + GSettingsFilename;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
	SettingsPath = FConfigCacheIni::NormalizeConfigIniPath(SettingsPath);
#endif
	return SettingsPath;
}

bool FBCConfigStore::LoadConfigContents(FString& OutContents)
{
	return FFileHelper::LoadFileToString(OutContents, *GetConfigPath());
}

bool FBCConfigStore::ResolveLegacy(FBCStoredCredentials& Out)
{
	if (ReadSetting(GAppIdKey).IsEmpty() || ReadSetting(GAppSecretKey).IsEmpty())
	{
		return false;
	}

	Out = FBCStoredCredentials();
	Out.AppId = ReadSetting(GAppIdKey);
	Out.ServerUrl = ReadSetting(GServerUrlKey);
	Out.Version = ReadSetting(GVersionKey);

	FBCStoredChildApp Child;
	Child.AppId = ReadSetting(GChildAppIdKey);
	if (!Child.AppId.IsEmpty() && !ReadSetting(GChildAppSecretKey).IsEmpty())
	{
		Out.ChildApps.Add(Child);
	}
	return true;
}

EBCConfigState FBCConfigStore::DetectState()
{
	FString Contents;
	if (LoadConfigContents(Contents))
	{
#if BC_NATIVE_CONFIG
		return LoadNativeConfig(Contents).IsValid() ? EBCConfigState::Ready : EBCConfigState::Invalid;
#else
		return EBCConfigState::Invalid;
#endif
	}

	FBCStoredCredentials Legacy;
	if (ResolveLegacy(Legacy))
	{
		return EBCConfigState::Legacy;
	}
	return ReadSetting(GUnsupportedVersionKey).IsEmpty() ? EBCConfigState::None : EBCConfigState::Invalid;
}

bool FBCConfigStore::Resolve(FBCStoredCredentials& Out)
{
	FString Contents;
	if (!LoadConfigContents(Contents))
	{
		return ResolveLegacy(Out);
	}

#if BC_NATIVE_CONFIG
	TSharedPtr<braincloud::NativeConfig> Config = LoadNativeConfig(Contents);
	if (Config.IsValid())
	{
		ReadNativeConfig(*Config, Out);
		return true;
	}
#endif
	return false;
}

bool FBCConfigStore::ResolveAppProfiles(FBCStoredCredentials& OutCredentials, TMap<FString, FBCAppProfile>& OutAppProfiles)
{
	OutAppProfiles.Empty();

#if BC_NATIVE_CONFIG
	FString Contents;
	if (!LoadConfigContents(Contents))
	{
		return false;
	}

	TSharedPtr<const braincloud::NativeConfig> Config = LoadNativeConfig(Contents);
	if (!Config.IsValid())
	{
		return false;
	}

	ReadNativeConfig(*Config, OutCredentials);

	TArray<FString> AppIds = { OutCredentials.AppId };
	for (const FBCStoredChildApp& Child : OutCredentials.ChildApps)
	{
		AppIds.Add(Child.AppId);
	}
	for (const FString& AppId : AppIds)
	{
		const std::string NativeAppId = ToStd(AppId);
		OutAppProfiles.Add(AppId, [Config, NativeAppId](const TArray<uint8>& Payload)
		{
			return FromStd(Config->Sign(NativeAppId, std::vector<uint8_t>(Payload.GetData(), Payload.GetData() + Payload.Num())));
		});
	}
	return true;
#else
	return false;
#endif
}

FString FBCConfigStore::ResolveAppId()
{
	FBCStoredCredentials Credentials;
	Resolve(Credentials);
	return Credentials.AppId;
}

TArray<FString> FBCConfigStore::ResolveChildAppIds()
{
	FBCStoredCredentials Credentials;
	Resolve(Credentials);

	TArray<FString> Ids;
	for (const FBCStoredChildApp& Child : Credentials.ChildApps)
	{
		Ids.Add(Child.AppId);
	}
	return Ids;
}

FString FBCConfigStore::ResolveServerUrl()
{
	FBCStoredCredentials Credentials;
	Resolve(Credentials);
	return Credentials.ServerUrl;
}

bool FBCConfigStore::ResolveDebugLogging()
{
	return ReadSetting(GDebugLoggingKey).ToBool();
}

void FBCConfigStore::StoreDebugLogging(bool bEnabled)
{
	const FString SettingsPath = GetSettingsPath();
	if (!FPaths::FileExists(SettingsPath))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *SettingsPath);
	}
	GConfig->LoadFile(SettingsPath);
	GConfig->SetBool(GCredentialsSection, GDebugLoggingKey, bEnabled, SettingsPath);
	GConfig->Flush(false, SettingsPath);
}
