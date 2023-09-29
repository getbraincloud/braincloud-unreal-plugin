// Copyright 2018 bitHeads, Inc. All Rights Reserved.

using System.Collections.Generic;
using System;
using System.IO;
using UnrealBuildTool;
public class BCCompatibility : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

#if WITH_FORWARDED_MODULE_RULES_CTOR
    public BCCompatibility(ReadOnlyTargetRules Target) : base(Target)
#else
    public BCCompatibility(TargetInfo Target)
#endif
    {
        PrivatePCHHeaderFile = "Private/BCCompatibilityPCH.h";
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine"
                });

        // win64
        PublicDefinitions.Add("PLATFORM_UWP=0");
    }
}
