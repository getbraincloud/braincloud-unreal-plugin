// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCSecureStore.generated.h"

/**
 * The state of the app credentials stored in Config/BrainCloudSettings.ini.
 */
UENUM(BlueprintType)
enum class EBCCredentialState : uint8
{
	/** No credentials are stored. */
	None UMETA(DisplayName = "None"),
	/** Credentials are stored in plain text. */
	Legacy UMETA(DisplayName = "Legacy (plaintext)"),
	/** Credentials are stored and can be read. */
	Secure UMETA(DisplayName = "Secure"),
	/** Credentials are stored but cannot be read, and need to be set again. */
	Invalid UMETA(DisplayName = "Invalid")
};

/**
 * A child app of the stored app.
 */
USTRUCT(BlueprintType)
struct BCCLIENTPLUGIN_API FBCStoredChildApp
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppId;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppSecret;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppName;
};

/**
 * The app credentials stored in Config/BrainCloudSettings.ini.
 */
USTRUCT(BlueprintType)
struct BCCLIENTPLUGIN_API FBCStoredCredentials
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppId;

	UPROPERTY(BlueprintReadOnly, Category = "BrainCloud")
	FString AppSecret;

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

	bool IsValid() const { return !AppId.IsEmpty() && !AppSecret.IsEmpty(); }
};

/**
 * Reads and writes the app credentials stored in Config/BrainCloudSettings.ini.
 * Credentials are normally set in the editor with Tools > brainCloud.
 */
class BCCLIENTPLUGIN_API FBCSecureStore
{
public:
	/**
	 * Returns the state of the stored credentials.
	 */
	static EBCCredentialState DetectState();

	/**
	 * Reads the stored credentials.
	 *
	 * @param Out The stored credentials
	 * @return True if an app id and app secret are stored
	 */
	static bool Resolve(FBCStoredCredentials& Out);

	/**
	 * Stores the app credentials.
	 *
	 * @param AppId The app's id
	 * @param AppSecret The app's secret
	 * @param AppName The app's name
	 * @param ServerUrl The url to the brainCloud server. Left unchanged if empty
	 * @param OutError The reason the credentials could not be stored
	 * @return True if the credentials were stored
	 */
	static bool Store(const FString& AppId, const FString& AppSecret, const FString& AppName,
		const FString& ServerUrl, FString& OutError);

	/**
	 * Replaces the stored child apps. The parent app must be stored first.
	 *
	 * @param Children The child apps to store
	 * @param OutError The reason the child apps could not be stored
	 * @return True if the child apps were stored
	 */
	static bool StoreChildren(const TArray<FBCStoredChildApp>& Children, FString& OutError);

	/**
	 * Removes all stored child apps.
	 */
	static void ClearChildren();

	/**
	 * Returns the ids of the stored child apps.
	 */
	static TArray<FString> ResolveChildAppIds();

	/**
	 * Stores credentials that are in plain text in the current format.
	 *
	 * @param OutError The reason the credentials could not be stored
	 * @return True if the credentials were stored
	 */
	static bool MigrateLegacy(FString& OutError);

	/**
	 * Returns the stored app name.
	 */
	static FString ResolveAppName();

	/**
	 * Stores the app name.
	 *
	 * @param AppId The app's id
	 * @param AppName The app's name
	 * @return True if the app name was stored
	 */
	static bool StoreAppName(const FString& AppId, const FString& AppName);

	/**
	 * Returns the stored app id.
	 */
	static FString ResolveAppId();

	/**
	 * Returns the stored url to the brainCloud server.
	 */
	static FString ResolveServerUrl();

	/**
	 * Returns true if debug logging is enabled for the stored app.
	 */
	static bool ResolveDebugLogging();

	/**
	 * Enables or disables debug logging for the stored app.
	 *
	 * @param bEnabled True to enable debug logging
	 */
	static void StoreDebugLogging(bool bEnabled);

	/**
	 * Removes all stored credentials.
	 */
	static void Clear();

	static FString EncodeValue(const FString& Value) { return EncodeSimple(Value); }
	static FString DecodeValue(const FString& Encoded) { return DecodeSimple(Encoded); }

private:
	static FString GetConfigPath();

	static FString ReadSecret(const FString& ConfigPath, const FString& SlotSection, const FString& PlainKey);
	static void WriteSecretSlots(const FString& ConfigPath, const FString& SlotSection, const TArray<FString>& Slots);
	static TArray<FBCStoredChildApp> ReadChildren(const FString& ConfigPath, bool bEncoded);
	static void RemoveChildKeys(const FString& ConfigPath);

	static TArray<FString> EncodeSecret(const FString& Secret);
	static FString DecodeSecret(const TArray<FString>& Slots);

	static FString EncodeSimple(const FString& Value);
	static FString DecodeSimple(const FString& Encoded);
};
