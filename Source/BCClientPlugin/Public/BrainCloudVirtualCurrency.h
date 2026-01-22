// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudVirtualCurrency
{
public:
     BrainCloudVirtualCurrency(BrainCloudClient *client);

     /**
      * Retrieve the user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
      *
      * Service Name - VirtualCurrency
      * Service Operation - GetCurrency
      *
      * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
      * @param in_callback The method to be invoked when the server response is received
      */
     void getCurrency(const FString &in_vcId, IServerCallback *callback = nullptr);

     /**
      * Retrieve the parent user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
      *
      * Service Name - VirtualCurrency
      * Service Operation - GetParentCurrency
      *
      * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
      * @param in_levelName The parent level name
      * @param in_callback The method to be invoked when the server response is received
      */
     void getParentCurrency(const FString &in_vcId, const FString &in_levelName, IServerCallback *callback = nullptr);

     /**
      * Retrieve the peer user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
      *
      * Service Name - VirtualCurrency
      * Service Operation - GetPeerCurrency
      *
      * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
      * @param in_peerCode The peer code identifying the other user
      * @param in_callback The method to be invoked when the server response is received
      */
     void getPeerCurrency(const FString &in_vcId, const FString &in_peerCode, IServerCallback *callback = nullptr);

     /**
      * Reset player's currency to zero
      *
      * Service Name - VirtualCurrency
      * Service Operation - ResetCurrency
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void resetCurrency(IServerCallback *callback = nullptr);

     /**
      * @warning Method is recommended to be used in Cloud Code only for security
      * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
      * @param in_currencyType The currency type to award
      * @param in_amount The amount to award
      * @param in_callback The method to be invoked when the server response is received
      */
     void awardCurrency(const FString &currencyType, int32 amount, IServerCallback *callback);

     /**
      * @warning Method is recommended to be used in Cloud Code only for security
      * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
      * @param in_currencyType The currency type to consume
      * @param in_amount The amount to consume
      * @param in_callback The method to be invoked when the server response is received
      */
     void consumeCurrency(const FString &currencyType, int32 amount, IServerCallback *callback);

private:
     BrainCloudClient *_client = nullptr;
};
