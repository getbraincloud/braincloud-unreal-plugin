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

        // Writing the app configuration is editor only: games link the read-only library alone.
        string NativeRoot = Path.Combine(ModulePath, "..", "BCClientPlugin", "ThirdParty", "BrainCloudNative");
        string WriterLib = null;
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            WriterLib = Path.Combine(NativeRoot, "lib", "Win64", "BrainCloudNativeWriter.lib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            WriterLib = Path.Combine(NativeRoot, "lib", "Mac", "BrainCloudNativeWriter-Mac.a");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            WriterLib = Path.Combine(NativeRoot, "lib", "Linux", "libBrainCloudNativeWriter.a");
        }
        if (WriterLib == null || !File.Exists(WriterLib))
        {
            throw new BuildException("brainCloud: missing native writer library for " + Target.Platform.ToString() + ": " + (WriterLib ?? "unsupported platform"));
        }
        PrivateIncludePaths.Add(Path.Combine(NativeRoot, "include"));
        PublicAdditionalLibraries.Add(WriterLib);

        // win64
        PublicDefinitions.Add("PLATFORM_UWP=0");
    }
}
