// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCCampaignProxy.h"
#include "BCClientPluginPrivatePCH.h"
#include "BCWrapperProxy.h"
#include "BrainCloudClient.h"
#include "ServerCall.h"

#include "BrainCloudWrapper.h"

UBCCampaignProxy::UBCCampaignProxy(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBCCampaignProxy *UBCCampaignProxy::GetMyCampaigns(UBrainCloudWrapper *brainCloudWrapper, const FString &optionsJson)
{
	UBCCampaignProxy *Proxy = NewObject<UBCCampaignProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getCampaignService()->getMyCampaigns(optionsJson, Proxy);
	return Proxy;
}
