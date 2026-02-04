// Copyright 2015 bitHeads, Inc. All Rights Reserved.


#include "BCReconnectCallbackProxy.h"
#include "BCClientPluginPrivatePCH.h"
#include "BCWrapperProxy.h"
#include "BrainCloudWrapper.h"

UBCReconnectCallbackProxy::UBCReconnectCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBCReconnectCallbackProxy* UBCReconnectCallbackProxy::RegisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCReconnectCallbackProxy* Proxy = NewObject<UBCReconnectCallbackProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->registerReconnectCallback(Proxy);
	return Proxy;
}

void UBCReconnectCallbackProxy::DeregisterReconnectCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->deregisterReconnectCallback();
}
