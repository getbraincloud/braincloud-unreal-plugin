// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudAppStore
{
public:
  BrainCloudAppStore(BrainCloudClient *client);

  /**
   * Verifies that purchase was properly made at the store.
   *
   * Service Name - appStore
   * Service Operation - VERIFY_PURCHASE
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param jsonReceiptData The specific store data required
   * @param callback The method to be invoked when the server response is received
   */
  void verifyPurchase(const FString &in_storeId, const FString &in_jsonReceiptData, IServerCallback *callback = nullptr);

  /**
   * Returns the eligible promotions for the player.
   *
   * Service Name - appStore
   * Service Operation - ELIGIBLE_PROMOTIONS
   *
   * @param callback The method to be invoked when the server response is received
   */
  void getEligiblePromotions(IServerCallback *callback = nullptr);

  /**
   * Method gets the active sales inventory for the passed-in
   * currency type.
   *
   * Service Name - appStore
   * Service Operation - GET_INVENTORY
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param userCurrency The currency type to retrieve the sales inventory for.
   * @param callback The method to be invoked when the server response is received
   */
  void getSalesInventory(const FString &in_storeId, const FString &in_userCurrency, IServerCallback *callback = nullptr);

  /**
   * Method gets the active sales inventory for the passed-in
   * currency type.
   *
   * Service Name - appStore
   * Service Operation - GET_INVENTORY
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param userCurrency The currency type to retrieve the sales inventory for.
   * @param category The product category
   * @param callback The method to be invoked when the server response is received
   */
  void getSalesInventoryByCategory(const FString &in_storeId, const FString &in_userCurrency, const FString &in_category, IServerCallback *callback = nullptr);

  /**
   * Start A Two Staged Purchase Transaction
   *
   * Service Name - appStore
   * Service Operation - START_PURCHASE
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param jsonPurchaseData Specific data for starting a two-stage purchase
   * @param callback The method to be invoked when the server response is received
   */
  void startPurchase(const FString &in_storeId, const FString &in_jsonPurchaseData, IServerCallback *callback = nullptr);

  /**
   * Before making a purchase with the IAP store, you will need to store the purchase
   * payload context on brainCloud so that the purchase can be verified for the proper IAP product.
   * This payload will be used during the VerifyPurchase method to ensure the
   * user properly paid for the correct product before awarding them the IAP product.
   *
   * Service Name - appStore
   * Service Operation - CACHE_PURCHASE_PAYLOAD_CONTEXT
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param iapId The IAP product id as configured on brainCloud
   * @param payload The payload retrieved for the IAP product
   * @param callback The method to be invoked when the server response is received
   */
  void cachePurchasePayloadContext(const FString &in_storeId, const FString &iapId, const FString &payload, IServerCallback *callback = nullptr);

  /**
   * Finalize A Two Staged Purchase Transaction
   *
   * Service Name - appStore
   * Service Operation - FINALIZE_PURCHASE
   *
   * @param storeId The store platform. Valid stores are:
   * - itunes
   * - facebook
   * - appworld
   * - steam
   * - windows
   * - windowsPhone
   * - googlePlay
   * - metaHorizon
   * @param transactionId The transaction id returned from startPurchase
   * @param jsonTransactionData Specific transaction data for finalizing purchase
   * @param callback The method to be invoked when the server response is received
   */
  void finalizePurchase(const FString &in_storeId, const FString &in_transactionId, const FString &in_jsonTransactionData, IServerCallback *callback = nullptr);

  /**
   * Returns up-to-date eligible 'promotions' for the user and a 'promotionsRefreshed' flag indicating whether the user's promotion info required refreshing.
   *
   * Service Name - appStore
   * Service Operation - REFRESH_PROMOTIONS
   * @param callback The method to be invoked when the server response is received
   */
  void refreshPromotions(IServerCallback *callback = nullptr);

private:
  BrainCloudClient *_client = nullptr;
};
