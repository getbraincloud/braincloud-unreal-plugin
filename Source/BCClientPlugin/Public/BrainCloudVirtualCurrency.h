// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudVirtualCurrency
{
public:
    BrainCloudVirtualCurrency(BrainCloudClient *client);

    /**
     * Retrieve the user's currency account. Optional parameter: `vcId` (if retrieving a specific currency).
     *
     * Service Name - virtualCurrency
     * Service Operation - GET_PLAYER_VC
     *
     * @param vcId Optional currency id to retrieve (pass NULL to get all currencies)
     * @param callback The method to be invoked when the server response is received
     */
    void getCurrency(const FString &in_vcId, IServerCallback *callback = nullptr);

    /**
     * Retrieve the parent user's currency account. Optional parameter: `vcId` (if retrieving a specific currency).
     *
     * Service Name - virtualCurrency
     * Service Operation - GET_PARENT_VC
     *
     * @param vcId Optional currency id to retrieve (pass NULL to get all currencies)
     * @param levelName The parent level name
     * @param callback The method to be invoked when the server response is received
     */
    void getParentCurrency(const FString &in_vcId, const FString &in_levelName, IServerCallback *callback = nullptr);

    /**
     * Retrieve the peer user's currency account. Optional parameter: `vcId` (if retrieving a specific currency).
     *
     * Service Name - virtualCurrency
     * Service Operation - GET_PEER_VC
     *
     * @param vcId Optional currency id to retrieve (pass NULL to get all currencies)
     * @param peerCode The peer code identifying the other user
     * @param callback The method to be invoked when the server response is received
     */
    void getPeerCurrency(const FString &in_vcId, const FString &in_peerCode, IServerCallback *callback = nullptr);

    /**
     * Reset player's currency to zero
     *
     * Service Name - virtualCurrency
     * Service Operation - RESET_PLAYER_VC
     *
     * @param callback The method to be invoked when the server response is received
     */
    void resetCurrency(IServerCallback *callback = nullptr);

    /**
     * @warning Method is recommended to be used in Cloud Code only for security
     * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
     * @param currencyType The currency type to award
     * @param amount The amount to award
     * @param callback The method to be invoked when the server response is received
     */
    void awardCurrency(const FString &currencyType, int32 amount, IServerCallback *callback);

    /**
     * @warning Method is recommended to be used in Cloud Code only for security
     * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
     * @param currencyType The currency type to consume
     * @param amount The amount to consume
     * @param callback The method to be invoked when the server response is received
     */
    void consumeCurrency(const FString &currencyType, int32 amount, IServerCallback *callback);

private:
    BrainCloudClient *_client = nullptr;
};
