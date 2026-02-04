// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintRestCallProxyBase.h"
#include "BCLongSessionCallbackProxy.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UBCLongSessionCallbackProxy : public UBCBlueprintRestCallProxyBase
{
	GENERATED_BODY()
public:
	UBCLongSessionCallbackProxy(const FObjectInitializer& ObjectInitializer);

	/**
	*
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Client")
	static UBCLongSessionCallbackProxy* RegisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);

	/** 
	*   
	*/
	UFUNCTION(BlueprintCallable, Category = "BrainCloud|Client")
	static void DeregisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper);
};
