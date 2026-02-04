// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintRestCallProxyBase.h"
#include "BCReconnectCallbackProxy.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UBCReconnectCallbackProxy : public UBCBlueprintRestCallProxyBase
{
	GENERATED_BODY()
public:
	UBCReconnectCallbackProxy(const FObjectInitializer& ObjectInitializer);

	/**
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Client")
	static UBCReconnectCallbackProxy* RegisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);

	/**
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud|Client")
	static void DeregisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);
};
