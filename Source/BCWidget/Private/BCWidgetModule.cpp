// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#include "BCWidgetModule.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Misc/ConfigCacheIni.h"

IMPLEMENT_MODULE(FBCWidgetModule, BCWidget);
DEFINE_LOG_CATEGORY(LogBCWidget);

void FBCWidgetModule::StartupModule()
{
    UE_LOG(LogBCWidget, Log, TEXT("BrainCloud Widget Startup!"));
    
#if WITH_EDITOR && WITH_EDITORONLY_DATA
    FString SectionName = TEXT("/Script/Blutility.EditorUtilitySubsystem");
    FString Filename = TEXT("DefaultEditorPerProjectUserSettings.ini");

    FString ConfigPath = FPaths::ProjectConfigDir();
    ConfigPath += Filename;

#if ENGINE_MAJOR_VERSION == 5
    ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + *Filename);
#endif

    if (GConfig) {
        FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);
        FConfigFile* ConfigFile = GConfig->FindConfigFile(*ConfigPath);

        if (!GConfig->DoesSectionExist(*SectionName, ConfigPath))
        {
            GConfig->EmptySection(*SectionName, ConfigPath);
        }
#if ENGINE_MAJOR_VERSION == 5
        GConfig->SetString(*SectionName, TEXT("+StartupObjects"), TEXT("/BCClient/EditorUtility/BCUtilityWidget.BCUtilityWidget"), ConfigPath);
#elif ENGINE_MAJOR_VERSION == 4
        GConfig->SetString(*SectionName, TEXT("+StartupObjects"), TEXT("/BCClient/EditorUtility/BCUtilityWidget_UE4.BCUtilityWidget_UE4"), ConfigPath);
#endif
    }

    GConfig->Flush(true, ConfigPath);

#if ENGINE_MAJOR_VERSION == 5

    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
    FToolMenuSection& NewSection = Menu->AddSection("BrainCloud", FText::FromString("BrainCloud"));

    NewSection.AddSeparator(FName("BrainCloud"));
    NewSection.AddMenuEntry(
        FName("BrainCloud"),
        FText::FromString("BrainCloud Settings"),
        FText::FromString("Change BrainCloud App Settings"),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateRaw(this, &FBCWidgetModule::MenuCommand)
        ),
        EUserInterfaceActionType::Button,
        FName("BrainCloud Utilities"));

#elif ENGINE_MAJOR_VERSION == 4

    UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
    FToolMenuSection& NewSection = Menu->AddSection("BrainCloud", FText::FromString("BrainCloud"));

    FToolMenuEntry toolbarEntry = FToolMenuEntry::InitToolBarButton(
        FName("BrainCloud Settings"),
        FUIAction(
            FExecuteAction::CreateRaw(this, &FBCWidgetModule::MenuCommand)
        ),
        FText::FromString("BrainCloud Settings"),
        FText::FromString("Change BrainCloud Settings"),
        FSlateIcon(),
        EUserInterfaceActionType::Button,
        FName("BrainCloud Utilities")
    );
    NewSection.AddEntry(toolbarEntry);
#endif
#endif
}

void FBCWidgetModule::ShutdownModule()
{
}

void FBCWidgetModule::MenuCommand()
{
#if WITH_EDITOR && WITH_EDITORONLY_DATA 
    FString Command;

#if ENGINE_MAJOR_VERSION == 5
    Command = TEXT("KISMETEVENT BCUtilityWidget_C_0 OpenWidgetOptions");
#elif ENGINE_MAJOR_VERSION == 4
    Command = TEXT("KISMETEVENT BCUtilityWidget_UE4_C_0 OpenWidgetOptions");
#endif
    FString Output;

    // Ensure we are in the context of a world
    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (World)
    {
        GEngine->Exec(World, *Command);
    }
#endif
}
