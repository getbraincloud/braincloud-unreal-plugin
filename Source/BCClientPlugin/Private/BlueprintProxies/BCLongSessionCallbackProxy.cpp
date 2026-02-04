// Copyright 2015 bitHeads, Inc. All Rights Reserved.


#include "BCLongSessionCallbackProxy.h"
#include "BCClientPluginPrivatePCH.h"
#include "BCWrapperProxy.h"
#include "BrainCloudWrapper.h"

UBCLongSessionCallbackProxy::UBCLongSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBCLongSessionCallbackProxy* UBCLongSessionCallbackProxy::RegisterLongSessionCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCLongSessionCallbackProxy* Proxy = NewObject<UBCLongSessionCallbackProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->registerLongSessionCallback(Proxy);
	return Proxy;
}

void UBCLongSessionCallbackProxy::DeregisterLongSessionCallback(UBrainCloudWrapper* brainCloudWrapper)
{
	UBCWrapperProxy::GetBrainCloudInstance(brainCloudWrapper)->getBCClient()->deregisterLongSessionCallback();
}
