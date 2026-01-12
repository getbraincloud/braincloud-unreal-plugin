// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintCallProxyBase.h"
#include "BCAuthType.h"
#include "BCUserItemsProxy.generated.h"

UCLASS(MinimalAPI)
class UBCUserItemsProxy : public UBCBlueprintCallProxyBase
{
	GENERATED_BODY()

  public:
	UBCUserItemsProxy(const FObjectInitializer &ObjectInitializer);

	/*
    * Allows item(s) to be awarded to a user without collecting
	* the purchase amount. If includeDef is true, response 
	* includes associated itemDef with language fields limited
	* to the current or default language.
    *
    * Service Name - UserItems
    * Service Operation - AWARD_USER_ITEM
    *
    * @param defId
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *AwardUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &defId, int quantity, bool includeDef);

    /*
    * Allows item(s) to be awarded to a user without collecting
    * the purchase amount. If includeDef is true, response
    * includes associated itemDef with language fields limited
    * to the current or default language.
    *
    * Service Name - UserItems
    * Service Operation - AWARD_USER_ITEM_WITH_OPTIONS
    *
    * @param defId The unique id of the item definition to award.
    * @param quantity The quantity of the item to award.
    * @param includeDef If true, the associated item definition will be included in the response.
    * @param optionsJson Optional support for specifying 'blockIfExceedItemMaxStackable' indicating how to process the award
    * if the defId is for a stackable item with a max stackable quantity and the specified quantity to award is too high.
    * If true and the quantity is too high, the call is blocked and an error is returned.
    * If false (default) and quantity is too high, the quantity is adjusted to the allowed maximum and the quantity not awarded is reported
    * in response key 'itemsNotAwarded' - unless the adjusted quantity would be 0, in which case the call is blocked and an error is returned.
    * @param callback The method to be invoked when the server response is received
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
    static UBCUserItemsProxy* AwardUserItemWithOptions(UBrainCloudWrapper* brainCloudWrapper, const FString& defId, int quantity, bool includeDef, const FString& optionsJson);

    /*
    * Allows a quantity of a specified user item to be dropped, 
    * without any recovery of the money paid for the item. If any 
    * quantity of the user item remains, it will be returned, potentially 
    * with the associated itemDef (with language fields limited to the 
    * current or default language).
    *
    * Service Name - UserItems
    * Service Operation - DROP_USER_ITEM
    *
    * @param itemId
    * @param quantity
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *DropUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int quantity, bool includeDef);

    /*
    * Returns list of items on promotion available to the current user
    * for promotions available to the current user.
    *
    * Service Name - UserItems
    * Service Operation - GET_ITEMS_ON_PROMOTION
    *
    * @param shopId The id identifying the store the item is from, if applicable.
    * @param includeDef If true, the associated item definition info of the promotional items will be included in the response.
    * @param includePromotionDetails If true, the promotion details of the eligible promotions will be included in the response
	* @param optionsJson Optional support for specifying additional options. Currently supporting option 'category' to include only catalog items configured with the specified category, if desired.
    * @param callback The method to be invoked when the server response is received
    *
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
    static UBCUserItemsProxy* GetItemsOnPromotion(UBrainCloudWrapper* brainCloudWrapper, const FString& shopId, bool includeDef, bool includePromotionDetails, const FString& optionsJson = TEXT(""));

    /*
    * Returns list of promotional details for the specified item definition
    * for promotions available to the current user.
    *
    * Service Name - UserItems
    * Service Operation - GET_ITEMS_ON_PROMOTION
    *
    * @param defId The unique id of the item definition to check.
    * @param shopId The id identifying the store the item is from, if applicable.
    * @param includeDef If true, the associated item definition will be included in the response.
    * @param includePromotionDetails If true, the promotion details of the eligible promotions will be included in the response.
    * @param callback The method to be invoked when the server response is received
    *
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
    static UBCUserItemsProxy* GetItemPromotionDetails(UBrainCloudWrapper* brainCloudWrapper, const FString& defId, const FString& shopId, bool includeDef, bool includePromotionDetails);

  /*
    * Retrieves the page of user's inventory from the server 
    * based on the context. If includeDef is true, response includes
    * associated itemDef with each user item, with language fields 
    * limited to the current or default language.
    *
    * Service Name - UserItems
    * Service Operation - GET_USER_ITEMS_PAGE
    *
    * @param criteria
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *GetUserItemsPage(UBrainCloudWrapper *brainCloudWrapper, const FString &context, bool includeDef);

  /*
    * Retrieves the page of user's inventory 
    * from the server based on the encoded context. 
    * If includeDef is true, response includes associated 
    * itemDef with each user item, with language fields limited 
    * to the current or default language.
    *
    * Service Name - UserItems
    * Service Operation - GET_USER_ITEMS_PAGE_OFFSET
    *
    * @param criteria
    * @param pageOffset
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *GetUserItemsPageOffset(UBrainCloudWrapper *brainCloudWrapper, const FString &context, int pageOffset, bool includeDef);
    
	/*
    * Retrieves the identified user item from the server.
    * If includeDef is true, response includes associated
    * itemDef with language fields limited to the current 
    * or default language.
    *
    * Service Name - UserItems
    * Service Operation - GET_USER_ITEM
    *
    * @param itemId
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *GetUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, bool includeDef);

    /*
    * Gifts item to the specified player.
    *
    * Service Name - UserItems
    * Service Operation - GIVE_USER_ITEM_TO
    *
    * @param itemId
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *GiveUserItemTo(UBrainCloudWrapper *brainCloudWrapper, const FString &profileId, const FString &itemId, int version, int quantity, bool immediate);
    
	/*
    * Gifts item to the specified player.
    *
    * Service Name - UserItems
    * Service Operation - PURCHASE_USER_ITEM
    *
    * @param itemId
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *PurchaseUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &defId, int quantity, const FString &shopId, bool includeDef);
   
    /*
    * Purchases a quantity of an item from the specified store,
    * if the user has enough funds and purchasing for listed buy price
    * is not disabled for associated catalog item definition.
    * If includeDef is true, response includes associated itemDef with language fields
    * limited to the current or default language.
    *
    * Service Name - UserItems
    * Service Operation - PURCHASE_USER_ITEM
    *
    * @param defId The unique id of the item definition to purchase.
    * @param quantity The quantity of the item to purchase.
    * @param shopId The id identifying the store the item is being purchased from, if applicable.
    * @param includeDef If true, the associated item definition will be included in the response.
    * @param optionsJson Optional support for specifying 'blockIfExceedItemMaxStackable' indicating how to process the purchase
    * if the defId is for a stackable item with a max stackable quantity and the specified quantity being purchased is too high.
    * If true and the quantity is too high, the call is blocked and an error is returned. If false (default) and quantity is too high,
    * the quantity is adjusted to the allowed maximum and the quantity not purchased is reported in response key 'itemsNotPurchased'
    * - unless the adjusted quantity would be 0, in which case the call is blocked and an error is returned.
    * @param callback The method to be invoked when the server response is received
    *
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
    static UBCUserItemsProxy* PurchaseUserItemsWithOptions(
        UBrainCloudWrapper* brainCloudWrapper,
        const FString& defId,
        int quantity,
        const FString& shopId,
        bool includeDef,
        const FString& optionsJson
    );

    /*
    * Retrieves and transfers the gift item from the specified player, 
    * who must have previously called giveUserItemTo.
    *
    * Service Name - UserItems
    * Service Operation - RECEIVE_USER_ITEM_FROM
    *
    * @param profileId
    * @param itemId
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *ReceiveUserItemFrom(UBrainCloudWrapper *brainCloudWrapper, const FString &profileId, const FString &itemId);
   
    /*
    * Allows a quantity of a specified user item to be sold. 
    * If any quantity of the user item remains, 
    * it will be returned, potentially with the associated 
    * itemDef (with language fields limited to the current 
    * or default language), along with the currency refunded 
    * and currency balances.
    *
    * Service Name - UserItems
    * Service Operation - SELL_USER_ITEM
    *
    * @param itemId
    * @param version
    * @param quantity
    * @param shopId
    * @param includeDef
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *SellUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int version, int quantity,const FString &shopId, bool includeDef);
    
	/*
    * Updates the item data on the specified user item.
    *
    * Service Name - UserItems
    * Service Operation - UPDATE_USER_ITEM_DATA
    *
    * @param itemId
    * @param version
    * @param newItemData
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *UpdateUserItemData(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int version, const FString &newItemData);
    
	/*
    * Uses the specified item, potentially consuming it.
    *
    * Service Name - UserItems
    * Service Operation - USE_USER_ITEM
    *
    * @param itemId
    * @param version
    * @param newItemData
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *UseUserItem(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int version, const FString &newItemData, bool includeDef);

    /*
    * Publishes the specified item to the item management attached blockchain. Results are reported asynchronously via an RTT event.
    *
    * Service Name - UserItems
    * Service Operation - PUBLISH_USER_ITEM_TO_BLOCKCHAIN
    *
    * @param itemId
    * @param version
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *PublishUserItemToBlockchain(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int version);
    
    /*
    * Syncs the caller's user items with the item management attached blockchain. Results are reported asynchronously via an RTT event.
    *
    * Service Name - UserItems
    * Service Operation - REFRESH_BLOCKCHAIN_USER_ITEMS
    *
    * @param callback The method to be invoked when the server response is received
    */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *RefreshBlockchainUserItems(UBrainCloudWrapper *brainCloudWrapper);

    /*
    * Removes the specified item from the item management attached blockchain. Results are reported asynchronously via an RTT event.    *
    * Service Name - UserItems
    * Service Operation - REMOVE_USER_ITEM_FROM_BLOCKCHAIN
    *
    * @param itemId
    * @param version
    * @param callback The method to be invoked when the server response is received
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|UserItems")
	static UBCUserItemsProxy *RemoveUserItemFromBlockchain(UBrainCloudWrapper *brainCloudWrapper, const FString &itemId, int version);

};
