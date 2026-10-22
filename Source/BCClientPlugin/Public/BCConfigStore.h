// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCAppProfile.h"
#include "BCConfigStore.generated.h"

/**
 * The state of the app configuration set in the editor with Tools > brainCloud.
 */
UENUM(BlueprintType)
enum class EBCConfigState : uint8
{
	/** No app is configured. */
	None UMETA(DisplayName = "None"),
	/** Credentials are stored in plain text in Config/BrainCloudSettings.ini. */
	Legacy UMETA(DisplayName = "Legacy (plaintext)"),
	/** An app is configured and can be used. */
	Ready UMETA(DisplayName = "Ready"),
	/** An app is configured but cannot be read, and needs to be set again. */
	Invalid UMETA(DisplayName = "Invalid")
};

/**
 * A child app of the configured app.
 */
USTRUCT(BlueprintType)
struct BCCLIENTPLUGIN_API FBCStoredChildApp
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppId;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppName;
};

/**
 * The app configured in the editor with Tools > brainCloud.
 */
USTRUCT(BlueprintType)
struct BCCLIENTPLUGIN_API FBCStoredCredentials
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppId;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppName;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString ServerUrl;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString Version;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	TArray<FBCStoredChildApp> ChildApps;

	const FBCStoredChildApp* FindChild(const FString& ChildAppId) const
	{
		return ChildApps.FindByPredicate([&ChildAppId](const FBCStoredChildApp& Child) { return Child.AppId == ChildAppId; });
	}

	bool IsValid() const { return !AppId.IsEmpty(); }
};

/**
 * Reads the app configuration set in the editor with Tools > brainCloud.
 */
class BCCLIENTPLUGIN_API FBCConfigStore
{
public:
	/**
	 * Returns true if app configurations can be read on this platform.
	 */
	static bool IsSupported();

	/**
	 * Returns the state of the app configuration.
	 */
	static EBCConfigState DetectState();

	/**
	 * Reads the configured app and child app ids, server url and version.
	 *
	 * @param Out The configured app
	 * @return True if an app is configured
	 */
	static bool Resolve(FBCStoredCredentials& Out);

	/**
	 * Reads the configured app and creates an app profile for it and each child app.
	 *
	 * @param OutCredentials The configured app
	 * @param OutAppProfiles The app profiles, by app id
	 * @return True if an app is configured and can be used
	 */
	static bool ResolveAppProfiles(FBCStoredCredentials& OutCredentials, TMap<FString, FBCAppProfile>& OutAppProfiles);

	/**
	 * Returns the configured app id.
	 */
	static FString ResolveAppId();

	/**
	 * Returns the configured child app ids.
	 */
	static TArray<FString> ResolveChildAppIds();

	/**
	 * Returns the configured url to the brainCloud server.
	 */
	static FString ResolveServerUrl();

	/**
	 * Returns true if debug logging is enabled.
	 */
	static bool ResolveDebugLogging();

	/**
	 * Enables or disables debug logging.
	 *
	 * @param bEnabled True to enable debug logging
	 */
	static void StoreDebugLogging(bool bEnabled);

	/**
	 * Returns the path to the app configuration, Config/BrainCloudNative.ini.
	 */
	static FString GetConfigPath();

	/**
	 * Returns the path to Config/BrainCloudSettings.ini.
	 */
	static FString GetSettingsPath();

	/**
	 * Reads plain text credentials from Config/BrainCloudSettings.ini.
	 *
	 * @param Out The app and child app ids, server url and version
	 * @return True if plain text credentials are stored
	 */
	static bool ResolveLegacy(FBCStoredCredentials& Out);

private:
	static bool LoadConfigContents(FString& OutContents);
};
