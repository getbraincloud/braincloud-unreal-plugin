// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "Engine/Engine.h"
#include "Editor.h"
#include "Modules/ModuleManager.h"
#include "Framework/Commands/UIAction.h"
#include "LevelEditorMenuContext.h"
#include "ToolMenus.h"
#include <LevelEditor.h>


class FBCClientPlugin : public IBCClientPlugin
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override
    {


        UE_LOG(LogBrainCloud, Log, TEXT("BrainCloud Plugin Startup"));

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
                FExecuteAction::CreateRaw(this, &FBCClientPlugin::MenuCommand)
            ),
            EUserInterfaceActionType::Button,
            FName("BrainCloud Utilities"));

#elif ENGINE_MAJOR_VERSION == 4

        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
        FToolMenuSection& NewSection = Menu->AddSection("BrainCloud", FText::FromString("BrainCloud"));

        FToolMenuEntry toolbarEntry = FToolMenuEntry::InitToolBarButton(
            FName("BrainCloud Settings"),
            FUIAction(
                FExecuteAction::CreateRaw(this, &FBCClientPlugin::MenuCommand)
            ),
            FText::FromString("BrainCloud Settings"),
            FText::FromString("Change BrainCloud Settings"),
            FSlateIcon(),
            EUserInterfaceActionType::Button,
            FName("BrainCloud Utilities")
        );
        NewSection.AddEntry(toolbarEntry);
#endif
    }

    void MenuCommand()
    {
        UE_LOG(LogBrainCloud, Log, TEXT("Execute Console Command"));
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
    }

    virtual void ShutdownModule() override
    {

    }

};
IMPLEMENT_MODULE(FBCClientPlugin, BCClientPlugin)

DEFINE_LOG_CATEGORY(LogBrainCloud);
DEFINE_LOG_CATEGORY(LogBrainCloudComms);
DEFINE_LOG_CATEGORY(LogBrainCloudRelayComms);
