// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintRestCallProxyBase.h"
#include "BCAutoReconnectCallbackProxy.generated.h"

/**
 *
 */
UCLASS(MinimalAPI)
class UBCAutoReconnectCallbackProxy : public UBCBlueprintRestCallProxyBase
{
	GENERATED_BODY()
public:
	UBCAutoReconnectCallbackProxy(const FObjectInitializer& ObjectInitializer);

	/**
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Client")
	static UBCAutoReconnectCallbackProxy* RegisterAutoReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);

	/**
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud|Client")
	static void DeregisterAutoReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);
};
