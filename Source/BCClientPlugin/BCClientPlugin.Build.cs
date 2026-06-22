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
                    "HTTP",
                    "AndroidNative"
                });

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
