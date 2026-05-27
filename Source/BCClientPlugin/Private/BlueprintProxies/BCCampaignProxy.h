// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintCallProxyBase.h"
#include "BrainCloudCampaign.h"
#include "BCCampaignProxy.generated.h"

UCLASS(MinimalAPI)
class UBCCampaignProxy : public UBCBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UBCCampaignProxy(const FObjectInitializer &ObjectInitializer);

	/**
	 * Returns the list of campaigns the current player is participating in,
	 * providing campaign, campaign scenario, and participation details.
	 *
	 * Service Name - Campaign
	 * Service Operation - GET_MY_CAMPAIGNS
	 *
	 * @param optionsJson Optional parameters as a JSON string (reserved for future use).
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Campaign")
	static UBCCampaignProxy *GetMyCampaigns(UBrainCloudWrapper *brainCloudWrapper, const FString &optionsJson);
};
