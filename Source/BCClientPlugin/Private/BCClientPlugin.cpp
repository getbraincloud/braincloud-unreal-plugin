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

        FString ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + *Filename);

        if (GConfig) {
            FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*SectionName, false, true, ConfigPath);
            FConfigFile* ConfigFile = GConfig->FindConfigFile(*ConfigPath);

            if (ConfigFile->DoesSectionExist(*SectionName))
            {
                UE_LOG(LogBrainCloud, Log, TEXT("Already has section"));
            }
            else {
                UE_LOG(LogBrainCloud, Log, TEXT("Does not have section, add it"));

                GConfig->EmptySection(*SectionName, ConfigPath);
                GConfig->SetString(*SectionName, TEXT("+StartupObjects"), TEXT("/BCClient/EditorUtility/BCUtilityWidget.BCUtilityWidget"), ConfigPath);
            }
        }

        GConfig->Flush(true, ConfigPath);

        //UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
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


        UE_LOG(LogBrainCloud, Log, TEXT("Added BC ToolBar button"));
    }

    void MenuCommand()
    {
        UE_LOG(LogBrainCloud, Log, TEXT("Execute Console Command"));

        FString Command = TEXT("KISMETEVENT BCUtilityWidget.BCUtilityWidget_C OpenWidgetOptions");
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
