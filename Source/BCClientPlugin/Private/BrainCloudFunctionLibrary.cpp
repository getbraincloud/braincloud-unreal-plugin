// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudFunctionLibrary.h"
#include "BCClientPluginPrivatePCH.h"

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

    GConfig->SetString(*SectionName, TEXT("AppId"), *appData.AppId, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("AppSecret"), *appData.AppSecret, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("Version"), *appData.Version, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("ServerUrl"), *appData.ServerUrl, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("S2SKey"), *appData.S2SKey, ConfigPath);
    GConfig->SetString(*SectionName, TEXT("S2SUrl"), *appData.S2SUrl, ConfigPath);

    GConfig->Flush(false, ConfigPath);

    UE_LOG(LogBrainCloud, Warning, TEXT("App Data saved to config file, please restart Unreal Editor for changes to take effect"));
}
