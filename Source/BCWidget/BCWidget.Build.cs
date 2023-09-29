// Copyright 2018 bitHeads, Inc. All Rights Reserved.

using System.Collections.Generic;
using System;
using System.IO;
using UnrealBuildTool;
public class BCWidget : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

#if WITH_FORWARDED_MODULE_RULES_CTOR
    public BCWidget(ReadOnlyTargetRules Target) : base(Target)
#else
    public BCWidget(TargetInfo Target)
#endif
    {
        PrivatePCHHeaderFile = "Private/BCWidgetPrivatePCH.h";
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "ToolMenus",
                    "Slate",
                    "SlateCore"
                });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorStyle" });
        }

        // win64
        PublicDefinitions.Add("PLATFORM_UWP=0");
    }
}
