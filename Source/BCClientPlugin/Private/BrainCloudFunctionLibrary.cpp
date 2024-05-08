// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudFunctionLibrary.h"
#include "BCClientPluginPrivatePCH.h"
#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Launch/Resources/Version.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "AndroidNativeLibrary.h"
#include <iostream>


#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h" // Include this to allow using Windows API
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h" // Include this to hide Windows API usa
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#endif

#if PLATFORM_IOS || PLATFORM_MAC

#include <Foundation/Foundation.h>

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
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
        const FConfigSection* ConfigSection = GConfig->GetSection(*SectionName, false, ConfigPath);
#else
        FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);
#endif

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

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
    const FConfigSection* ConfigSection = GConfig->GetSection(*SectionName, false, ConfigPath);
#else
    FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);
#endif

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
#if PLATFORM_MAC || PLATFORM_IOS

    NSLocale* currentLocale = [NSLocale currentLocale];
    if (currentLocale != nil) {
        NSString* countryCode = [currentLocale objectForKey : NSLocaleCountryCode];
        if (countryCode != nil) {
            CountryCode = FString(countryCode);
        }
    }

#elif PLATFORM_ANDROID
    CountryCode = UAndroidNativeLibrary::GetCountryCode();
#elif PLATFORM_WINDOWS

    int geoId = GetUserGeoID(16);
    int lcid = GetUserDefaultLCID();
    wchar_t locationBuffer[3];
    GetGeoInfo(geoId, 4, locationBuffer, 3, lcid);

    CountryCode = locationBuffer;
#else
    CountryCode = FInternationalization::Get().GetCurrentLocale()->GetRegion();
#endif

    if (CountryCode.IsEmpty()) {
        //fall back to current/active culture if empty result
        CountryCode = FInternationalization::Get().GetCurrentLocale()->GetRegion();
    }
    if (CountryCode.IsEmpty()) {
        // fall back to PlatformMisc default locale if still empty result
        CountryCode = SplitCountryCodeFromLocale(FPlatformMisc::GetDefaultLocale());
    }

    return  FormatCountryCode(CountryCode);
}

FString UBrainCloudFunctionLibrary::SplitCountryCodeFromLocale(FString locale)
{
    FString CountryCode("");

    // on some platforms, may come back like "es-419" or "en-GB" or "zh-Hans" so parse it out
    // on some platforms, may come back like with underscore seperator like "en_US"
    FString language, country;
    locale.Split(TEXT("-"), &language, &country);

    if (country.IsEmpty()) {
        locale.Split(TEXT("_"), &language, &country);
        CountryCode = country;
    }
    else {
        CountryCode = country;
    }

    // by default, just use the passed in value
    if (CountryCode.IsEmpty()) {
        CountryCode = locale;
    }
    return CountryCode;
}

FString UBrainCloudFunctionLibrary::GetCountryCodeFromCulture(FString locale)
{
    FString CountryCode("");

    // this locale won't get a region code
    if ((locale.ToLower() == "zh-hans") || (locale.ToLower() == "zh-hant")) {
        locale += "-CN";
    }

    // on some platforms, will come back like "es-419" or "en-GB" so parse Region out
    // note using Unreal FCulturePtr class to get the region works in most cases
    // returns en-US-POSIX when it can't find a culture though (eg. empty string, gibberish)
    // will be invalid if it's a number or something
    // alternatively, we could split the string on "-"
    FCulturePtr culture = FInternationalization::Get().GetCulture(locale);

    if (culture.IsValid()) {
        CountryCode = culture->GetRegion();    
    }

    // by default, just use the passed in value
    if (CountryCode.IsEmpty()) {
        CountryCode = locale;
    }
    return CountryCode;
}

FString UBrainCloudFunctionLibrary::FormatCountryCode(FString InputCode)
{
    FString CountryCode = InputCode;
    if (CountryCode == "419") {
        CountryCode = "_LA_";
    }
    else if ((CountryCode == "Hans") || (CountryCode == "Hant")) {
        CountryCode = "CN";
    }
    else if(CountryCode != "_LA_") {
        CountryCode = CountryCode.ToUpper().Left(2);
    }
    return CountryCode;
}

FString UBrainCloudFunctionLibrary::GetSystemLanguageCode()
{
    FString LanguageCode = FString();

    LanguageCode = FInternationalization::Get().GetCurrentCulture()->GetName();

    if (LanguageCode.IsEmpty())
        LanguageCode = FPlatformMisc::GetDefaultLanguage();

    return LanguageCode;
}


