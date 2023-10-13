// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FBCCompatibilityModule : public IModuleInterface
{
public:

    FBCCompatibilityModule() {}

    virtual ~FBCCompatibilityModule() {}

    void RemoveUE5Content();

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
