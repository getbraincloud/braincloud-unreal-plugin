// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "CoreMinimal.h"

class FBCClientPlugin : public IBCClientPlugin
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override
    {
        UE_LOG(LogBrainCloud, Log, TEXT("BrainCloud Plugin Startup"));
    }

    virtual void ShutdownModule() override
    {

    }

};
IMPLEMENT_MODULE(FBCClientPlugin, BCClientPlugin)

DEFINE_LOG_CATEGORY(LogBrainCloud);
DEFINE_LOG_CATEGORY(LogBrainCloudComms);
DEFINE_LOG_CATEGORY(LogBrainCloudRelayComms);
