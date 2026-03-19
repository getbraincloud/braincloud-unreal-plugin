// Copyright 2015 bitHeads, Inc. All Rights Reserved.


#include "BCAutoReconnectCallbackProxy.h"
#include "BCClientPluginPrivatePCH.h"
#include "BCWrapperProxy.h"
#include "BrainCloudWrapper.h"

UBCAutoReconnectCallbackProxy::UBCAutoReconnectCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBCAutoReconnectCallbackProxy* UBCAutoReconnectCallbackProxy::RegisterAutoReconnectCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCAutoReconnectCallbackProxy* Proxy = NewObject<UBCAutoReconnectCallbackProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->registerAutoReconnectCallback(Proxy);
	return Proxy;
}

void UBCAutoReconnectCallbackProxy::DeregisterAutoReconnectCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->deregisterAutoReconnectCallback();
}
