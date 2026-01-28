// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudBlockchain
{
public:
    BrainCloudBlockchain(BrainCloudClient *in_client);

    /**
     * Retrieves the blockchain items owned by the caller.
     *
     * Service Name - blockchain
     * Service Operation - GET_BLOCKCHAIN_ITEMS
     *
     * @param integrationID  The blockchain integration id.
     *                          Currently only 'default' is supported.
     * @param contextJson    Optional. Reserved for future use.
     * @param callback       The callback handler
     */
    void GetBlockchainItems(FString in_integrationID, FString in_contextJson, IServerCallback *in_callback);
    /**
     * Retrieves the uniqs owned by the caller.
     *
     * Service Name - blockchain
     * Service Operation - GET_UNIQS
     *
     * @param integrationID  The blockchain integration id.
     *                          Currently only 'default' is supported.
     * @param contextJson    Optional. Reserved for future use.
     * @param callback       The callback handler
     */
    void GetUniqs(FString in_integrationID, FString in_contextJson, IServerCallback *in_callback);

private:
    BrainCloudClient *_client = nullptr;
};
