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
                    "SlateCore",
                    "UMG",
                    "Blutility",
                    "HTTP",
                    "Json",
                    "JsonUtilities",
                    "BCClientPlugin"
                });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "EditorStyle" });
        }

        bool bOAuthSupported = Target.Version.MajorVersion == 5 ||
            (Target.Version.MajorVersion == 4 && Target.Version.MinorVersion >= 24);

        if (bOAuthSupported)
        {
            PublicDependencyModuleNames.Add("HTTPServer");
        }
        PublicDefinitions.Add("BC_WIDGET_OAUTH_SUPPORTED=" + (bOAuthSupported ? "1" : "0"));

        // win64
        PublicDefinitions.Add("PLATFORM_UWP=0");
    }
}
