// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudCampaign
{
public:
	BrainCloudCampaign(BrainCloudClient *client);

	/**
	 * Returns the list of campaigns the current player is participating in,
	 * providing campaign, campaign scenario, and participation details.
	 *
	 * Service Name - Campaign
	 * Service Operation - GET_MY_CAMPAIGNS
	 *
	 * @param optionsJson Optional parameters as a JSON string (reserved for future use).
	 * @param callback The method to be invoked when the server response is received.
	 */
	void getMyCampaigns(const FString &optionsJson, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
};
