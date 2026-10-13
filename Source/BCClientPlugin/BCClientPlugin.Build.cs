// Copyright 2018 bitHeads, Inc. All Rights Reserved.
#if !UE_4_24_OR_LATER
#define EARLIER_THAN_4_23
#endif

using System.Collections.Generic;
using System;
using System.IO;
using UnrealBuildTool;
public class BCClientPlugin : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

#if WITH_FORWARDED_MODULE_RULES_CTOR
    public BCClientPlugin(ReadOnlyTargetRules Target) : base(Target)
#else
    public BCClientPlugin(TargetInfo Target)
#endif
    {
        PrivatePCHHeaderFile = "Private/BCClientPluginPrivatePCH.h";
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(
            new string[] {
                    Path.Combine(ModulePath,"Private"),
                    Path.Combine(ModulePath,"Private/BlueprintProxies")
                });

        PrivateDependencyModuleNames.AddRange(
            new string[]
                {
                    "JsonUtilities",
                    "HTTP"
                });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("AndroidNative");
        }

        PublicDependencyModuleNames.AddRange(
            new string[]
                {
                    "Core",
                    "CoreUObject",
                    "ApplicationCore",
                    "Engine",
                    "Sockets",
                    "Networking",
                    "WebSockets",
                    "Json",
                    "HTTP"
                });

        PublicDefinitions.Add("PLATFORM_UWP=0");

        string SecureRoot = Path.Combine(ModulePath, "ThirdParty", "BrainCloudNative");
        string SecureLibRoot = Path.Combine(SecureRoot, "lib");
        string PlatformName = Target.Platform.ToString();
        List<string> SecureLibs = new List<string>();
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "Win64", "BrainCloudNative.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "Mac", "libBrainCloudNative.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "IOS", "libBrainCloudNative.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "Android", "arm64-v8a", "libBrainCloudNative.a"));
            SecureLibs.Add(Path.Combine(SecureLibRoot, "Android", "x86_64", "libBrainCloudNative.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "Linux", "libBrainCloudNative.a"));
        }
        else if (PlatformName == "LinuxArm64" || PlatformName == "LinuxAArch64")
        {
            SecureLibs.Add(Path.Combine(SecureLibRoot, "LinuxArm64", "libBrainCloudNative.a"));
        }
        SecureLibs.RemoveAll(Lib => !File.Exists(Lib));

        bool bHasSecureLib = SecureLibs.Count > 0;
        if (bHasSecureLib)
        {
            PrivateIncludePaths.Add(Path.Combine(SecureRoot, "include"));
            PublicAdditionalLibraries.AddRange(SecureLibs);
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicSystemLibraries.Add("bcrypt.lib");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicFrameworks.Add("Security");
            }
        }
        PublicDefinitions.Add("BC_SECURE_NATIVE=" + (bHasSecureLib ? "1" : "0"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDependencyModuleNames.Add("zlib");
        }

    #if EARLIER_THAN_4_23
    #if WITH_FORWARDED_MODULE_RULES_CTOR
        else if (Target.Platform == UnrealTargetPlatform.HTML5)
        {
            PublicLibraryPaths.Add(Path.Combine(ModulePath, "ThirdParty/lib/HTML5"));
            PublicAdditionalLibraries.Add(Path.Combine(ModulePath,"ThirdParty/lib/HTML5/WebSocket.js"));
        }
    #endif
    #endif
    }
}
