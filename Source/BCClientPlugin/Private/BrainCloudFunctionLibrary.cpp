// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudFunctionLibrary.h"

#if PLATFORM_IOS || PLATFORM_MAC
#define FVector __AppleCarbonFVector
#include <Foundation/Foundation.h>
#undef FVector
#endif

#include "BCClientPluginPrivatePCH.h"
#include "BCSecureStore.h"
#include "CoreMinimal.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Launch/Resources/Version.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#if PLATFORM_ANDROID
#include "AndroidNativeLibrary.h"
#endif
#include <iostream>
#include "Misc/FileHelper.h"


#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h" // Include this to allow using Windows API
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h" // Include this to hide Windows API usa
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#endif

FBrainCloudAppDataStruct UBrainCloudFunctionLibrary::GetBCAppData()
{
    FBrainCloudAppDataStruct Result;

    FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("BrainCloudSettings.ini");
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
    ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(ConfigPath);
#endif

    if (GConfig && FPaths::FileExists(ConfigPath))
    {
        GConfig->LoadFile(ConfigPath);
        const TCHAR* Section = TEXT("Credentials");
        GConfig->GetString(Section, TEXT("ServerUrl"), Result.ServerUrl, ConfigPath);
        GConfig->GetString(Section, TEXT("S2SKey"), Result.S2SKey, ConfigPath);
        GConfig->GetString(Section, TEXT("S2SUrl"), Result.S2SUrl, ConfigPath);
    }
    else
    {
        UE_LOG(LogBrainCloud, Warning, TEXT("Couldn't find BrainCloudSettings.ini file in projects Config folder"));
    }
    Result.Version = UBrainCloudFunctionLibrary::GetProjectVersion();

    FBCStoredCredentials Stored;
    FBCSecureStore::Resolve(Stored);
    Result.AppId = Stored.AppId;
    Result.AppSecret = Stored.AppSecret;
    if (Stored.ChildApps.Num() > 0)
    {
        Result.ChildAppId = Stored.ChildApps[0].AppId;
        Result.ChildAppSecret = Stored.ChildApps[0].AppSecret;
    }
    return Result;
}

void UBrainCloudFunctionLibrary::SetBCAppData(FBrainCloudAppDataStruct appData)
{
    FString ConfigPath = FPaths::ProjectConfigDir() + TEXT("BrainCloudSettings.ini");
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
    ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(ConfigPath);
#endif
    if (!FPaths::FileExists(ConfigPath))
    {
        FFileHelper::SaveStringToFile(TEXT(""), *ConfigPath);
    }
    GConfig->LoadFile(ConfigPath);

    const FString ServerFullUrl = appData.ServerUrl + "/dispatcherv2";
    const FString S2SFullUrl = appData.ServerUrl + "/s2sdispatcher";
    const TCHAR* Section = TEXT("Credentials");
    GConfig->SetString(Section, TEXT("Version"), *UBrainCloudFunctionLibrary::GetProjectVersion(), ConfigPath);
    GConfig->SetString(Section, TEXT("S2SKey"), *appData.S2SKey, ConfigPath);
    GConfig->SetString(Section, TEXT("S2SUrl"), *S2SFullUrl, ConfigPath);
    GConfig->Flush(false, ConfigPath);

    FBCStoredCredentials Existing;
    FBCSecureStore::Resolve(Existing);
    FString Error;
    if (!FBCSecureStore::Store(appData.AppId, appData.AppSecret,
            appData.AppId == Existing.AppId ? Existing.AppName : FString(), ServerFullUrl, Error))
    {
        UE_LOG(LogBrainCloud, Error, TEXT("SetBCAppData: %s"), *Error);
        return;
    }

    if (!appData.ChildAppId.IsEmpty())
    {
        TArray<FBCStoredChildApp> Children = Existing.ChildApps;
        FBCStoredChildApp* Child = Children.FindByPredicate(
            [&appData](const FBCStoredChildApp& C) { return C.AppId == appData.ChildAppId; });
        if (!Child)
        {
            Child = &Children.AddDefaulted_GetRef();
            Child->AppId = appData.ChildAppId;
        }
        Child->AppSecret = appData.ChildAppSecret;
        if (!FBCSecureStore::StoreChildren(Children, Error))
        {
            UE_LOG(LogBrainCloud, Error, TEXT("SetBCAppData: %s"), *Error);
        }
    }

    UE_LOG(LogBrainCloud, Warning, TEXT("App Data saved to config file at %s, please restart Unreal Editor for changes to take effect"), *ConfigPath);
}

FString UBrainCloudFunctionLibrary::GetAppId()
{
    return FBCSecureStore::ResolveAppId();
}

FString UBrainCloudFunctionLibrary::GetChildAppId()
{
    const TArray<FString> Ids = FBCSecureStore::ResolveChildAppIds();
    return Ids.Num() > 0 ? Ids[0] : FString();
}

TArray<FString> UBrainCloudFunctionLibrary::GetChildAppIds()
{
    return FBCSecureStore::ResolveChildAppIds();
}

FString UBrainCloudFunctionLibrary::GetEnvironment()
{
    const FString ServerUrl = FBCSecureStore::ResolveServerUrl();
    return GetEnvironmentFromUrl(ServerUrl.IsEmpty() ? TEXT("https://api.braincloudservers.com/dispatcherv2") : ServerUrl);
}

FString UBrainCloudFunctionLibrary::GetEnvironmentFromUrl(const FString& ServerUrl)
{
    const FString Domain = TEXT("braincloudservers.com");

    FString Host = ServerUrl.TrimStartAndEnd();
    int32 SchemeEnd = Host.Find(TEXT("://"));
    if (SchemeEnd != INDEX_NONE)
    {
        Host.RightChopInline(SchemeEnd + 3);
    }
    int32 HostEnd = INDEX_NONE;
    if (Host.FindChar(TEXT('/'), HostEnd))
    {
        Host.LeftInline(HostEnd);
    }
    if (Host.FindChar(TEXT(':'), HostEnd))
    {
        Host.LeftInline(HostEnd);
    }
    Host.ToLowerInline();

    if (Host != Domain && !Host.EndsWith(TEXT(".") + Domain))
    {
        return ServerUrl;
    }
    FString Env = Host.LeftChop(Domain.Len());
    Env.RemoveFromEnd(TEXT("."));
    Env.RemoveFromStart(TEXT("api."));
    if (Env == TEXT("api"))
    {
        Env.Empty();
    }
    return Env.IsEmpty() ? TEXT("prod") : Env;
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

FString UBrainCloudFunctionLibrary::GetProjectVersion()
{
    FString AppVersion;
    GConfig->GetString(
        TEXT("/Script/EngineSettings.GeneralProjectSettings"),
        TEXT("ProjectVersion"),
        AppVersion,
        GGameIni
    );

    return AppVersion;
}

FString UBrainCloudFunctionLibrary::GetProjectEnvironment()
{
    return GetEnvironment();
}


