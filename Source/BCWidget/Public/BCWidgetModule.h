// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

/**
* Online subsystem module class  (BrainCloud Implementation)
* Code related to the loading of the BrainCloud module
*/
class FBCWidgetModule : public IModuleInterface
{
public:

    FBCWidgetModule() {}

    virtual ~FBCWidgetModule() {}

    void MenuCommand();

    // IModuleInterface

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool SupportsDynamicReloading() override
    {
        return false;
    }

    virtual bool SupportsAutomaticShutdown() override
    {
        return false;
    }
};
