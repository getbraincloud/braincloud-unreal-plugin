// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCEditorCredentialsStore.h"
#include "Containers/StringConv.h"
#include "Misc/Base64.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"

namespace
{
    const TCHAR* GSectionName = TEXT("EditorCredentials");
    const TCHAR* GFilename = TEXT("BCEditorSettings.ini");

    FString GetConfigPath()
    {
        FString ConfigPath = FPaths::ProjectConfigDir() + GFilename;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
        ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + FString(GFilename));
#endif
        return ConfigPath;
    }
}

FString BCEditorCredentialsStore::Obfuscate(const FString& PlainText)
{
    FTCHARToUTF8 Utf8(*PlainText);
    TArray<uint8> Bytes;
    Bytes.Append(reinterpret_cast<const uint8*>(Utf8.Get()), Utf8.Length());
    for (uint8& Byte : Bytes)
    {
        // Rotate right 5 bits - cosmetic only, reversed by Deobfuscate's rotate-left-5.
        Byte = uint8(((Byte >> 5) & 0x07) | ((Byte << 3) & 0xF8));
    }
    return FBase64::Encode(Bytes);
}

FString BCEditorCredentialsStore::Deobfuscate(const FString& Obfuscated)
{
    TArray<uint8> Bytes;
    if (Obfuscated.IsEmpty() || !FBase64::Decode(Obfuscated, Bytes))
    {
        return FString();
    }
    for (uint8& Byte : Bytes)
    {
        Byte = uint8(((Byte << 5) & 0xE0) | ((Byte >> 3) & 0x1F));
    }
    Bytes.Add(0);
    return FString(UTF8_TO_TCHAR(reinterpret_cast<const ANSICHAR*>(Bytes.GetData())));
}

FBCEditorCredentials BCEditorCredentialsStore::Load()
{
    FBCEditorCredentials Result;

    const FString ConfigPath = GetConfigPath();
    if (!FPaths::FileExists(ConfigPath))
    {
        return Result;
    }

    GConfig->LoadFile(ConfigPath);
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
    const FConfigSection* ConfigSection = GConfig->GetSection(GSectionName, false, ConfigPath);
#else
    FConfigSection* ConfigSection = GConfig->GetSectionPrivate(GSectionName, false, true, ConfigPath);
#endif

    if (ConfigSection)
    {
        TArray<FName> Keys;
        ConfigSection->GenerateKeyArray(Keys);

        if (Keys.Contains(TEXT("AdminEmail")))
        {
            Result.AdminEmail = Deobfuscate(ConfigSection->Find(TEXT("AdminEmail"))->GetValue());
        }
        if (Keys.Contains(TEXT("TeamId")))
        {
            Result.TeamId = Deobfuscate(ConfigSection->Find(TEXT("TeamId"))->GetValue());
        }
        if (Keys.Contains(TEXT("ApiKey")))
        {
            Result.ApiKey = Deobfuscate(ConfigSection->Find(TEXT("ApiKey"))->GetValue());
        }
    }

    GConfig->Flush(false, ConfigPath);
    return Result;
}

void BCEditorCredentialsStore::Save(const FBCEditorCredentials& Credentials)
{
    const FString ConfigPath = GetConfigPath();

    if (!FPaths::FileExists(ConfigPath))
    {
        FFileHelper::SaveStringToFile(TEXT(""), *ConfigPath);
    }

    GConfig->LoadFile(ConfigPath);

    if (GConfig->DoesSectionExist(GSectionName, ConfigPath))
    {
        GConfig->EmptySection(GSectionName, ConfigPath);
    }

    GConfig->SetString(GSectionName, TEXT("AdminEmail"), *Obfuscate(Credentials.AdminEmail), ConfigPath);
    GConfig->SetString(GSectionName, TEXT("TeamId"), *Obfuscate(Credentials.TeamId), ConfigPath);
    GConfig->SetString(GSectionName, TEXT("ApiKey"), *Obfuscate(Credentials.ApiKey), ConfigPath);

    GConfig->Flush(false, ConfigPath);
}

void BCEditorCredentialsStore::Clear()
{
    Save(FBCEditorCredentials());
}
