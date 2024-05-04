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
		Get the country code (region) part of IETF locale string
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString GetCountryCodeFromLocale(FString Locale);

	/**
		Format the country code as per braincCloud server standards
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString FormatCountryCode(FString CountryCode);

	/**
		Platform dependent get the language
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud Utility")
	static FString GetSystemLanguageCode();
};
