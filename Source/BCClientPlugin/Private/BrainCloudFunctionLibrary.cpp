// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudFunctionLibrary.h"
#include "BCClientPluginPrivatePCH.h"
#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Launch/Resources/Version.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include <iostream>


#if PLATFORM_WINDOWS
#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include "windows.h"
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"
#endif

FBrainCloudAppDataStruct UBrainCloudFunctionLibrary::GetBCAppData()
{
    FBrainCloudAppDataStruct Result;

    FString SectionName = TEXT("Credentials");
    FString Filename = TEXT("BrainCloudSettings.ini");

    FString AppIdKey = TEXT("AppId");
    FString AppSecretKey = TEXT("AppSecret");
    FString AppVersionKey = TEXT("Version");
    FString ServerUrlKey = TEXT("ServerUrl");
    FString S2SKeyKey = TEXT("S2SKey");
    FString S2SUrlKey = TEXT("S2SUrl");

    FString ConfigPath = FPaths::ProjectConfigDir();
    ConfigPath += Filename;

#if ENGINE_MAJOR_VERSION == 5

    ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + *Filename);

#endif

    if (GConfig) {
        FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);
        FConfigFile* ConfigFile = GConfig->FindConfigFile(*ConfigPath);

        TArray<FName> configKeys;

        if (ConfigFile && ConfigSection) {

            ConfigSection->GenerateKeyArray(configKeys);

            Result.AppId = configKeys.Contains(*AppIdKey) ? ConfigSection->Find(*AppIdKey)->GetValue() : TEXT("");
            Result.AppSecret = configKeys.Contains(*AppSecretKey) ? ConfigSection->Find(*AppSecretKey)->GetValue() : TEXT("");
            Result.Version = configKeys.Contains(*AppVersionKey) ? ConfigSection->Find(*AppVersionKey)->GetValue() : TEXT("");
            Result.ServerUrl = configKeys.Contains(*ServerUrlKey) ? ConfigSection->Find(*ServerUrlKey)->GetValue() : TEXT("");
            Result.S2SKey = configKeys.Contains(*S2SKeyKey) ? ConfigSection->Find(*S2SKeyKey)->GetValue() : TEXT("");
            Result.S2SUrl = configKeys.Contains(*S2SUrlKey) ? ConfigSection->Find(*S2SUrlKey)->GetValue() : TEXT("");
        }
        else {
            UE_LOG(LogBrainCloud, Warning, TEXT("Couldn't find BrainCloudSettings.ini file in projects Config folder"));
        }
    }

    GConfig->Flush(false, ConfigPath);

    return Result;
}

void UBrainCloudFunctionLibrary::SetBCAppData(FBrainCloudAppDataStruct appData)
{
    FString SectionName = TEXT("Credentials");
    FString Filename = TEXT("BrainCloudSettings.ini");

    FString ConfigPath = FPaths::ProjectConfigDir();
    ConfigPath += Filename;

    FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);

#if ENGINE_MAJOR_VERSION == 5
    ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + *Filename);
#endif

    if (GConfig->DoesSectionExist(*SectionName, ConfigPath))
    {
        GConfig->EmptySection(*SectionName, ConfigPath);
    }

    FString ServerFullUrl = appData.ServerUrl + "/dispatcherv2";
    FString S2SFullUrl = appData.ServerUrl + "/s2sdispatcher";

    GConfig->SetString(*SectionName, TEXT("AppId"), *appData.AppId, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("AppSecret"), *appData.AppSecret, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("Version"), *appData.Version, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("ServerUrl"), *ServerFullUrl, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("S2SKey"), *appData.S2SKey, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("S2SUrl"), *S2SFullUrl, ConfigPath);

    GConfig->Flush(false, ConfigPath);

    UE_LOG(LogBrainCloud, Warning, TEXT("App Data saved to config file, please restart Unreal Editor for changes to take effect"));
}

void UBrainCloudFunctionLibrary::CopyToClipboard(const FString& TextString)
{
    FPlatformApplicationMisc::ClipboardCopy(*TextString);
}

bool UBrainCloudFunctionLibrary::ValidateAndExtractURL(const FString& InputURL, FString& OutURL)
{
    if (InputURL.IsEmpty())
        return false;

    FURL ParsedURL(nullptr, *InputURL, TRAVEL_Absolute);
    // Check if the URL is valid
    if (ParsedURL.Valid)
    {
        // Construct the base URL without the path
        OutURL = FString::Printf(TEXT("%s://%s"), *ParsedURL.Protocol, *ParsedURL.Host);
        return true; // URL is valid
    }
    else
    {
        // URL is not valid
        return false;
    }
}

FString UBrainCloudFunctionLibrary::GetSystemCountryCode()
{
    FString CountryCode = FString();
#if PLATFORM_IOS
    CountryCode = FIOSPlatformMisc::GetDefaultLocale();
#elif PLATFORM_ANDROID
    CountryCode = FAndroidMisc::GetDefaultLocale();
#elif PLATFORM_WINDOWS
    LCID locale = GetUserDefaultLCID();

    // Get the country code
    const int bufferSize = 10;  // Adjust as needed
    TCHAR countryBuffer[bufferSize];
    if (GetLocaleInfo(locale, LOCALE_SISO3166CTRYNAME, countryBuffer, bufferSize) > 0) {
        CountryCode = FString(countryBuffer);
    }
#else
    CountryCode = FInternationalization::Get().GetCurrentLocale()->GetRegion();
#endif

    return CountryCode;
}
