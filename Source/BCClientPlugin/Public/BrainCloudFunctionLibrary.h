// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrainCloudAppDataStruct.h"
#include "BrainCloudFunctionLibrary.generated.h"

UCLASS()
class BCCLIENTPLUGIN_API UBrainCloudFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
		read the appdata from BrainCloudSettings.ini (call before initialize)
		Deprecated, will be removed in a future release.
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility",
		meta = (DeprecatedFunction, DeprecationMessage = "Use Init to initialize brainCloud, and Get App Id, Get Child App Ids and Get Environment to read the stored settings. App credentials are set in the editor with Tools > brainCloud."))
	static FBrainCloudAppDataStruct GetBCAppData();

	/**
		write given appdata to BrainCloudSettings.ini
		Deprecated, will be removed in a future release.
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility",
		meta = (DeprecatedFunction, DeprecationMessage = "Use Init to initialize brainCloud, and Get App Id, Get Child App Ids and Get Environment to read the stored settings. App credentials are set in the editor with Tools > brainCloud."))
	static void SetBCAppData(FBrainCloudAppDataStruct appData);

	/**
		returns the app id set in the brainCloud editor panel (Tools > brainCloud)
		empty if none is set
	*/
	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static FString GetAppId();

	/**
		returns the first child app id set in the brainCloud editor panel
		empty if none is set
	*/
	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static FString GetChildAppId();

	/**
		returns every child app id set in the brainCloud editor panel
	*/
	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static TArray<FString> GetChildAppIds();

	/**
		returns the brainCloud environment of the server url set in the brainCloud editor panel
		For example, "prod" for https://api.braincloudservers.com and "internal" for
		https://api.internal.braincloudservers.com. Other domains return the full server url.
	*/
	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static FString GetEnvironment();

	/**
		returns the brainCloud environment of the given server url, see GetEnvironment
	*/
	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static FString GetEnvironmentFromUrl(const FString& ServerUrl);

	/**
		utility to copy string to system clipboard
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static void CopyToClipboard(const FString &TextString);

	/**
		parses server url
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static bool ValidateAndExtractURL(const FString &InputURL, FString &OutURL);

	/**
		Platform dependent get the region
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString GetSystemCountryCode();

	/**
		creates a FCulturePtr to retieve the country code (region) part of IETF locale string
		works for various cultures
		For example, the tag en stands for English; es-419 for Latin American Spanish; rm-sursilv for Romansh Sursilvan;
		sr-Cyrl for Serbian written in Cyrillic script; nan-Hant-TW for Min Nan Chinese using traditional Han characters, as spoken in Taiwan;
		and gsw-u-sd-chzh for Z�rich German.

	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString GetCountryCodeFromCulture(FString locale);

	/**
		Splits off the country code (region) on a "-" or "_" given IETF locale string
		only works with: <language>_<country>
		or: <language>-<country>
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString SplitCountryCodeFromLocale(FString locale);

	/**
		Format the country code as per braincCloud server expectations
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString FormatCountryCode(FString CountryCode);

	/**
		Platform dependent get the language
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString GetSystemLanguageCode();

	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility")
	static FString GetProjectVersion();

	UFUNCTION(BlueprintPure, Category = "BrainCloud Utility",
		meta = (DeprecatedFunction, DeprecationMessage = "Use Get Environment instead."))
	static FString GetProjectEnvironment();
};
