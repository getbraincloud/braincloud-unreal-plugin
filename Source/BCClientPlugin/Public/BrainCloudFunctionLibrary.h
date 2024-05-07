// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <BrainCloudAppDataStruct.h>
#include "BrainCloudFunctionLibrary.generated.h"

UCLASS()
class BCCLIENTPLUGIN_API UBrainCloudFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
		read the appdata from BrainCloudSettings.ini (call before initialize)
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FBrainCloudAppDataStruct GetBCAppData();

	/**
		write given appdata to BrainCloudSettings.ini (used internally by BC widget)
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static void SetBCAppData(FBrainCloudAppDataStruct appData);

	/** 
		utility to copy string to system clipboard
	*/
    UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
    static void CopyToClipboard(const FString & TextString);

	/**
		parses server url
	*/
    UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static bool ValidateAndExtractURL(const FString& InputURL, FString& OutURL);

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
		and gsw-u-sd-chzh for Zürich German.

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
};
