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

        string NativeRoot = Path.Combine(ModulePath, "ThirdParty", "BrainCloudNative");
        string NativeLibRoot = Path.Combine(NativeRoot, "lib");
        string PlatformName = Target.Platform.ToString();
        List<string> NativeLibs = new List<string>();
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "Win64", "BrainCloudNative.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "Mac", "BrainCloudNative-Mac.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "iOS", "BrainCloudNative-iOS.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "Android", "arm64-v8a", "libBrainCloudNative.a"));
            NativeLibs.Add(Path.Combine(NativeLibRoot, "Android", "x86_64", "libBrainCloudNative.a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "Linux", "libBrainCloudNative.a"));
        }
        else if (PlatformName == "LinuxArm64" || PlatformName == "LinuxAArch64")
        {
            NativeLibs.Add(Path.Combine(NativeLibRoot, "LinuxArm64", "libBrainCloudNative.a"));
        }

        List<string> MissingLibs = NativeLibs.FindAll(Lib => !File.Exists(Lib));
        if (MissingLibs.Count > 0)
        {
            throw new BuildException("brainCloud: missing native library for " + PlatformName + ": " + string.Join(", ", MissingLibs));
        }

        bool bHasNativeConfig = NativeLibs.Count > 0;
        if (bHasNativeConfig)
        {
            PrivateIncludePaths.Add(Path.Combine(NativeRoot, "include"));
            PublicAdditionalLibraries.AddRange(NativeLibs);
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicSystemLibraries.Add("bcrypt.lib");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicFrameworks.Add("Security");
            }
        }
        PublicDefinitions.Add("BC_NATIVE_CONFIG=" + (bHasNativeConfig ? "1" : "0"));

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
