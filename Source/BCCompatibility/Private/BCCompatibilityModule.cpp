// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#include "BCCompatibilityModule.h"
#include "Runtime/Launch/Resources/Version.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


IMPLEMENT_MODULE(FBCCompatibilityModule, BCCompatibility);
DEFINE_LOG_CATEGORY(LogBCCompatibility);

void FBCCompatibilityModule::RemoveUE5Content()
{
#if ENGINE_MAJOR_VERSION == 4
    //delete UE5 widget asset file
    FString relativePath = TEXT("Plugins/BCClient/Content/EditorUtility/BCUtilityWidget.uasset");
    FString fullPath = FPaths::Combine(FPaths::ProjectDir(), relativePath);

    if (FPaths::ValidatePath(fullPath) && FPaths::FileExists(fullPath))
    {
        IFileManager& fManager = IFileManager::Get();
        fManager.Delete(*fullPath);
    }
#endif
}

void FBCCompatibilityModule::StartupModule()
{
    UE_LOG(LogBCCompatibility, Log, TEXT("BrainCloud Compatibility Startup!"));

    RemoveUE5Content();
    
}

void FBCCompatibilityModule::ShutdownModule()
{
}
