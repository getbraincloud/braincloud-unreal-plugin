// Copyright 2021 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Delegates/Delegate.h"
//#include "Http.h"
#include "Containers/Map.h"
#include "Containers/List.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogBrainCloud, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogBrainCloudComms, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogBrainCloudRelayComms, Log, All);

#include "IBCClientPlugin.h"

#include "BCClientPluginClasses.h"
