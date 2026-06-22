// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudCampaign.h"
#include "BCClientPluginPrivatePCH.h"

#include "BrainCloudClient.h"
#include "ServerCall.h"
#include "JsonUtil.h"

BrainCloudCampaign::BrainCloudCampaign(BrainCloudClient *client) : _client(client) {}

void BrainCloudCampaign::getMyCampaigns(const FString &optionsJson, IServerCallback *callback)
{
	TSharedRef<FJsonObject> message = MakeShareable(new FJsonObject());

	if (OperationParam::isOptionalParamValid(optionsJson))
	{
		message->SetObjectField(OperationParam::UserItemsOptionsJson.getValue(), JsonUtil::jsonStringToValue(optionsJson));
	}

	ServerCall *sc = new ServerCall(ServiceName::Campaign, ServiceOperation::GetMyCampaigns, message, callback);
	_client->sendRequest(sc);
}
