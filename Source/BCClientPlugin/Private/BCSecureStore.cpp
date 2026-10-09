// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCSecureStore.h"

#include "BCClientPluginPrivatePCH.h"
#include "Containers/StringConv.h"
#include "Misc/Base64.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"

#if BC_SECURE_NATIVE
THIRD_PARTY_INCLUDES_START
#include "braincloud/BrainCloudSecure.h"
THIRD_PARTY_INCLUDES_END
#endif

namespace
{
	const TCHAR* GFilename = TEXT("BrainCloudSettings.ini");

	const TCHAR* GCredentialsSection = TEXT("Credentials");

	const TCHAR* GSlotSection = TEXT("ClientTuning");

	const TCHAR* GSecureVersionKey = TEXT("SecureVersion");
	const TCHAR* GSecureVersion = TEXT("2.0.0");

	const TCHAR* GAppIdKey = TEXT("AppId");
	const TCHAR* GAppSecretKey = TEXT("AppSecret");
	const TCHAR* GAppNameKey = TEXT("AppName");
	const TCHAR* GServerUrlKey = TEXT("ServerUrl");
	const TCHAR* GVersionKey = TEXT("Version");
	const TCHAR* GDebugLoggingKey = TEXT("DebugLogging");

	const TCHAR* GChildCountKey = TEXT("ChildAppCount");
	const TCHAR* GChildAppIdKey = TEXT("ChildAppId");
	const TCHAR* GChildAppSecretKey = TEXT("ChildAppSecret");
	const TCHAR* GChildAppNameKey = TEXT("ChildAppName");
	const TCHAR* GChildSlotSection = TEXT("ChildTuning");

	const int32 GSlotCount = 32;
	const TCHAR* GSlotNames[GSlotCount] = {
		TEXT("Region"), TEXT("Locale"), TEXT("Channel"), TEXT("Segment"),
		TEXT("Zone"), TEXT("Realm"), TEXT("Cohort"), TEXT("Tier"),
		TEXT("Variant"), TEXT("Flavor"), TEXT("Edition"), TEXT("Track"),
		TEXT("Stage"), TEXT("Wave"), TEXT("Batch"), TEXT("Bucket"),
		TEXT("Shard"), TEXT("Cluster"), TEXT("Node"), TEXT("Ring"),
		TEXT("Sector"), TEXT("District"), TEXT("Domain"), TEXT("Cell"),
		TEXT("Grid"), TEXT("Lane"), TEXT("Slice"), TEXT("Layer"),
		TEXT("Tag"), TEXT("Label"), TEXT("Marker"), TEXT("Facet")
	};

#if BC_SECURE_NATIVE
	std::string ToStd(const FString& Value)
	{
		FTCHARToUTF8 Converted(*Value);
		return std::string(Converted.Get(), Converted.Length());
	}

	FString FromStd(const std::string& Value)
	{
		return FString(UTF8_TO_TCHAR(Value.c_str()));
	}
#else
	void WarnNoNativeLibrary()
	{
		static bool bWarned = false;
		if (!bWarned)
		{
			bWarned = true;
			UE_LOG(LogBrainCloud, Error, TEXT("brainCloud: stored credentials are not supported on this platform."));
		}
	}
#endif

	FString ReadKey(const FString& ConfigPath, const FString& Section, const FString& Key)
	{
		FString Value;
		GConfig->GetString(*Section, *Key, Value, ConfigPath);
		return Value;
	}
}

FString FBCSecureStore::GetConfigPath()
{
	FString ConfigPath = FPaths::ProjectConfigDir() + GFilename;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
	ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + FString(GFilename));
#endif
	return ConfigPath;
}

TArray<FString> FBCSecureStore::EncodeSecret(const FString& Secret)
{
	TArray<FString> Slots;
#if BC_SECURE_NATIVE
	if (braincloud::SlotCount() != GSlotCount)
	{
		UE_LOG(LogBrainCloud, Error, TEXT("brainCloud: BrainCloudSecure slot count %d does not match the plugin's %d."), braincloud::SlotCount(), GSlotCount);
		return Slots;
	}
	for (const std::string& Slot : braincloud::EncodeSecret(ToStd(Secret)))
	{
		Slots.Add(FromStd(Slot));
	}
#else
	WarnNoNativeLibrary();
#endif
	return Slots;
}

FString FBCSecureStore::DecodeSecret(const TArray<FString>& Slots)
{
#if BC_SECURE_NATIVE
	if (Slots.Num() != GSlotCount)
	{
		return FString();
	}
	std::vector<std::string> NativeSlots;
	NativeSlots.reserve(Slots.Num());
	for (const FString& Slot : Slots)
	{
		NativeSlots.push_back(ToStd(Slot));
	}
	return FromStd(braincloud::DecodeSecret(NativeSlots));
#else
	WarnNoNativeLibrary();
	return FString();
#endif
}

FString FBCSecureStore::EncodeSimple(const FString& Value)
{
#if BC_SECURE_NATIVE
	return Value.IsEmpty() ? FString() : FromStd(braincloud::EncodeSimpleValue(ToStd(Value)));
#else
	WarnNoNativeLibrary();
	return FString();
#endif
}

FString FBCSecureStore::DecodeSimple(const FString& Encoded)
{
#if BC_SECURE_NATIVE
	return Encoded.IsEmpty() ? FString() : FromStd(braincloud::DecodeSimpleValue(ToStd(Encoded)));
#else
	return FString();
#endif
}

namespace
{
	FString Indexed(const TCHAR* Base, int32 Index)
	{
		return FString::Printf(TEXT("%s%d"), Base, Index);
	}
}

FString FBCSecureStore::ReadSecret(const FString& ConfigPath, const FString& SlotSection, const FString& PlainKey)
{
	const FString Plain = ReadKey(ConfigPath, GCredentialsSection, PlainKey);
	if (!Plain.IsEmpty())
	{
		return Plain;
	}
	if (ReadKey(ConfigPath, GCredentialsSection, GSecureVersionKey).IsEmpty())
	{
		return FString();
	}

	TArray<FString> Slots;
	Slots.SetNum(GSlotCount);
	for (int32 i = 0; i < GSlotCount; ++i)
	{
		Slots[i] = ReadKey(ConfigPath, SlotSection, GSlotNames[i]);
	}
	return DecodeSecret(Slots);
}

void FBCSecureStore::WriteSecretSlots(const FString& ConfigPath, const FString& SlotSection, const TArray<FString>& Slots)
{
	if (GConfig->DoesSectionExist(*SlotSection, ConfigPath))
	{
		GConfig->EmptySection(*SlotSection, ConfigPath);
	}
	for (int32 i = 0; i < GSlotCount; ++i)
	{
		GConfig->SetString(*SlotSection, GSlotNames[i], *Slots[i], ConfigPath);
	}
}

TArray<FBCStoredChildApp> FBCSecureStore::ReadChildren(const FString& ConfigPath, bool bEncoded)
{
	TArray<FBCStoredChildApp> Children;
	auto ReadName = [&](const FString& Key)
	{
		const FString Raw = ReadKey(ConfigPath, GCredentialsSection, Key);
		return bEncoded ? DecodeSimple(Raw) : Raw;
	};

	const FString CountValue = ReadKey(ConfigPath, GCredentialsSection, GChildCountKey);
	if (!CountValue.IsEmpty())
	{
		const int32 Count = FCString::Atoi(*CountValue);
		for (int32 i = 0; i < Count; ++i)
		{
			FBCStoredChildApp Child;
			Child.AppId = ReadKey(ConfigPath, GCredentialsSection, Indexed(GChildAppIdKey, i));
			if (Child.AppId.IsEmpty())
			{
				continue;
			}
			Child.AppName = ReadName(Indexed(GChildAppNameKey, i));
			Child.AppSecret = ReadSecret(ConfigPath, Indexed(GChildSlotSection, i), Indexed(GChildAppSecretKey, i));
			Children.Add(Child);
		}
		return Children;
	}

	FBCStoredChildApp Single;
	Single.AppId = ReadKey(ConfigPath, GCredentialsSection, GChildAppIdKey);
	if (!Single.AppId.IsEmpty())
	{
		Single.AppName = ReadName(GChildAppNameKey);
		Single.AppSecret = ReadSecret(ConfigPath, GChildSlotSection, GChildAppSecretKey);
		Children.Add(Single);
	}
	return Children;
}

void FBCSecureStore::RemoveChildKeys(const FString& ConfigPath)
{
	auto RemoveSection = [&ConfigPath](const FString& Section)
	{
		if (GConfig->DoesSectionExist(*Section, ConfigPath))
		{
			GConfig->EmptySection(*Section, ConfigPath);
		}
	};

	const int32 Count = FCString::Atoi(*ReadKey(ConfigPath, GCredentialsSection, GChildCountKey));
	for (int32 i = 0; i < Count; ++i)
	{
		GConfig->RemoveKey(GCredentialsSection, *Indexed(GChildAppIdKey, i), ConfigPath);
		GConfig->RemoveKey(GCredentialsSection, *Indexed(GChildAppNameKey, i), ConfigPath);
		GConfig->RemoveKey(GCredentialsSection, *Indexed(GChildAppSecretKey, i), ConfigPath);
		RemoveSection(Indexed(GChildSlotSection, i));
	}
	GConfig->RemoveKey(GCredentialsSection, GChildCountKey, ConfigPath);
	GConfig->RemoveKey(GCredentialsSection, GChildAppIdKey, ConfigPath);
	GConfig->RemoveKey(GCredentialsSection, GChildAppNameKey, ConfigPath);
	GConfig->RemoveKey(GCredentialsSection, GChildAppSecretKey, ConfigPath);
	RemoveSection(GChildSlotSection);
}

EBCCredentialState FBCSecureStore::DetectState()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return EBCCredentialState::None;
	}

	GConfig->LoadFile(ConfigPath);

	const FString AppId = ReadKey(ConfigPath, GCredentialsSection, GAppIdKey);

	if (!ReadKey(ConfigPath, GCredentialsSection, GAppSecretKey).IsEmpty() ||
		!ReadKey(ConfigPath, GCredentialsSection, GChildAppSecretKey).IsEmpty())
	{
		return EBCCredentialState::Legacy;
	}

	if (!ReadKey(ConfigPath, GCredentialsSection, GSecureVersionKey).IsEmpty())
	{
		if (ReadSecret(ConfigPath, GSlotSection, GAppSecretKey).IsEmpty())
		{
			return EBCCredentialState::Invalid;
		}
		for (const FBCStoredChildApp& Child : ReadChildren(ConfigPath, true))
		{
			if (Child.AppSecret.IsEmpty())
			{
				return EBCCredentialState::Invalid;
			}
		}
		return EBCCredentialState::Secure;
	}

	return AppId.IsEmpty() ? EBCCredentialState::None : EBCCredentialState::Invalid;
}

bool FBCSecureStore::Resolve(FBCStoredCredentials& Out)
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return false;
	}

	GConfig->LoadFile(ConfigPath);

	const bool bEncoded = !ReadKey(ConfigPath, GCredentialsSection, GSecureVersionKey).IsEmpty();
	const FString RawName = ReadKey(ConfigPath, GCredentialsSection, GAppNameKey);

	Out.AppId = ReadKey(ConfigPath, GCredentialsSection, GAppIdKey);
	Out.AppName = bEncoded ? DecodeSimple(RawName) : RawName;
	Out.ServerUrl = ReadKey(ConfigPath, GCredentialsSection, GServerUrlKey);
	Out.Version = ReadKey(ConfigPath, GCredentialsSection, GVersionKey);
	Out.AppSecret = ReadSecret(ConfigPath, GSlotSection, GAppSecretKey);
	Out.ChildApps = ReadChildren(ConfigPath, bEncoded);

	return Out.IsValid();
}

bool FBCSecureStore::Store(const FString& AppId, const FString& AppSecret, const FString& AppName,
	const FString& ServerUrl, FString& OutError)
{
	if (AppId.IsEmpty() || AppSecret.IsEmpty())
	{
		OutError = TEXT("App ID and App Secret are both required.");
		return false;
	}

	const TArray<FString> Slots = EncodeSecret(AppSecret);
	if (Slots.Num() != GSlotCount)
	{
		OutError = TEXT("Stored credentials are not supported on this platform. Credentials were not saved.");
		return false;
	}

	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *ConfigPath);
	}
	GConfig->LoadFile(ConfigPath);

	GConfig->SetString(GCredentialsSection, GAppIdKey, *AppId, ConfigPath);
	GConfig->SetString(GCredentialsSection, GAppNameKey, *EncodeSimple(AppName), ConfigPath);
	GConfig->SetString(GCredentialsSection, GSecureVersionKey, GSecureVersion, ConfigPath);
	if (!ServerUrl.IsEmpty())
	{
		GConfig->SetString(GCredentialsSection, GServerUrlKey, *ServerUrl, ConfigPath);
	}

	GConfig->RemoveKey(GCredentialsSection, GAppSecretKey, ConfigPath);
	WriteSecretSlots(ConfigPath, GSlotSection, Slots);
	GConfig->Flush(false, ConfigPath);

	if (ReadSecret(ConfigPath, GSlotSection, GAppSecretKey) != AppSecret)
	{
		OutError = FString::Printf(TEXT("Credentials were written to %s but did not read back correctly."), *ConfigPath);
		return false;
	}

	UE_LOG(LogBrainCloud, Log, TEXT("brainCloud credentials stored (encoded) in %s"), *ConfigPath);
	return true;
}

bool FBCSecureStore::StoreChildren(const TArray<FBCStoredChildApp>& Children, FString& OutError)
{
	TArray<TArray<FString>> EncodedSlots;
	for (const FBCStoredChildApp& Child : Children)
	{
		if (Child.AppId.IsEmpty() || Child.AppSecret.IsEmpty())
		{
			OutError = TEXT("Every child app needs an App ID and App Secret.");
			return false;
		}
		EncodedSlots.Add(EncodeSecret(Child.AppSecret));
		if (EncodedSlots.Last().Num() != GSlotCount)
		{
			OutError = TEXT("Stored credentials are not supported on this platform. Child credentials were not saved.");
			return false;
		}
	}

	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		OutError = TEXT("Save the parent app before its child apps.");
		return false;
	}
	GConfig->LoadFile(ConfigPath);
	if (ReadKey(ConfigPath, GCredentialsSection, GSecureVersionKey).IsEmpty())
	{
		OutError = TEXT("Save the parent app before its child apps.");
		return false;
	}

	RemoveChildKeys(ConfigPath);
	if (Children.Num() > 0)
	{
		GConfig->SetString(GCredentialsSection, GChildCountKey, *LexToString(Children.Num()), ConfigPath);
	}
	for (int32 i = 0; i < Children.Num(); ++i)
	{
		GConfig->SetString(GCredentialsSection, *Indexed(GChildAppIdKey, i), *Children[i].AppId, ConfigPath);
		GConfig->SetString(GCredentialsSection, *Indexed(GChildAppNameKey, i), *EncodeSimple(Children[i].AppName), ConfigPath);
		WriteSecretSlots(ConfigPath, Indexed(GChildSlotSection, i), EncodedSlots[i]);
	}
	GConfig->Flush(false, ConfigPath);

	const TArray<FBCStoredChildApp> ReadBack = ReadChildren(ConfigPath, true);
	bool bMatches = ReadBack.Num() == Children.Num();
	for (int32 i = 0; bMatches && i < Children.Num(); ++i)
	{
		bMatches = ReadBack[i].AppId == Children[i].AppId && ReadBack[i].AppSecret == Children[i].AppSecret;
	}
	if (!bMatches)
	{
		OutError = FString::Printf(TEXT("Child credentials were written to %s but did not read back correctly."), *ConfigPath);
		return false;
	}

	UE_LOG(LogBrainCloud, Log, TEXT("brainCloud: %d child app(s) stored (encoded) in %s"), Children.Num(), *ConfigPath);
	return true;
}

void FBCSecureStore::ClearChildren()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return;
	}
	GConfig->LoadFile(ConfigPath);
	RemoveChildKeys(ConfigPath);
	GConfig->Flush(false, ConfigPath);
}

TArray<FString> FBCSecureStore::ResolveChildAppIds()
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

bool FBCSecureStore::MigrateLegacy(FString& OutError)
{
	if (DetectState() != EBCCredentialState::Legacy)
	{
		OutError = TEXT("No plaintext credentials to migrate.");
		return false;
	}

	FBCStoredCredentials Current;
	Resolve(Current);

	if (!Store(Current.AppId, Current.AppSecret, Current.AppName, Current.ServerUrl, OutError))
	{
		return false;
	}
	if (Current.ChildApps.Num() > 0 && !StoreChildren(Current.ChildApps, OutError))
	{
		return false;
	}

	UE_LOG(LogBrainCloud, Log, TEXT("Migrated plaintext brainCloud credentials for app %s to the encoded form."), *Current.AppId);
	return true;
}

FString FBCSecureStore::ResolveAppName()
{
	FBCStoredCredentials Credentials;
	Resolve(Credentials);
	return Credentials.AppName;
}

bool FBCSecureStore::StoreAppName(const FString& AppId, const FString& AppName)
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return false;
	}
	GConfig->LoadFile(ConfigPath);
	GConfig->SetString(GCredentialsSection, GAppIdKey, *AppId, ConfigPath);
	GConfig->SetString(GCredentialsSection, GAppNameKey, *EncodeSimple(AppName), ConfigPath);
	GConfig->Flush(false, ConfigPath);
	return true;
}

FString FBCSecureStore::ResolveAppId()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return FString();
	}
	GConfig->LoadFile(ConfigPath);
	return ReadKey(ConfigPath, GCredentialsSection, GAppIdKey);
}

FString FBCSecureStore::ResolveServerUrl()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return FString();
	}
	GConfig->LoadFile(ConfigPath);
	return ReadKey(ConfigPath, GCredentialsSection, GServerUrlKey);
}

bool FBCSecureStore::ResolveDebugLogging()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return false;
	}
	GConfig->LoadFile(ConfigPath);
	bool bEnabled = false;
	GConfig->GetBool(GCredentialsSection, GDebugLoggingKey, bEnabled, ConfigPath);
	return bEnabled;
}

void FBCSecureStore::StoreDebugLogging(bool bEnabled)
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		FFileHelper::SaveStringToFile(TEXT(""), *ConfigPath);
	}
	GConfig->LoadFile(ConfigPath);
	GConfig->SetBool(GCredentialsSection, GDebugLoggingKey, bEnabled, ConfigPath);
	GConfig->Flush(false, ConfigPath);
}

void FBCSecureStore::Clear()
{
	const FString ConfigPath = GetConfigPath();
	if (!FPaths::FileExists(ConfigPath))
	{
		return;
	}
	GConfig->LoadFile(ConfigPath);
	RemoveChildKeys(ConfigPath);
	for (const TCHAR* Section : { GCredentialsSection, GSlotSection })
	{
		if (GConfig->DoesSectionExist(Section, ConfigPath))
		{
			GConfig->EmptySection(Section, ConfigPath);
		}
	}
	GConfig->Flush(false, ConfigPath);
}
