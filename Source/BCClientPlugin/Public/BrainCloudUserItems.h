// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudUserItems
{
public:
	BrainCloudUserItems(BrainCloudClient *client);

	/**
	 * Awards item(s) to a user without collecting the purchase amount.
	 * If includeDef is true, response includes associated itemDef
	 * with language fields limited to the current or default language.
	 *
	 * Service Name - userItems
	 * Service Operation - AWARD_USER_ITEM
	 *
	 * @param in_defId       The unique id of the item definition to award.
	 * @param in_quantity    The quantity of the item to award.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void awardUserItem(const FString &defId, int quantity, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Awards item(s) to a user with additional options.
	 *
	 * Service Name - userItems
	 * Service Operation - AWARD_USER_ITEM
	 *
	 * @param in_defId       The unique id of the item definition to award.
	 * @param in_quantity    The quantity of the item to award.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_optionsJson JSON string specifying additional options (e.g., blockIfExceedItemMaxStackable).
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void awardUserItemWithOptions(const FString &defId, int quantity, bool includeDef, const FString &optionsJson, IServerCallback *callback = nullptr);

	/**
	 * Drops a quantity of a specified user item without recovering the purchase cost.
	 * If any quantity remains, it may include the associated itemDef.
	 *
	 * Service Name - userItems
	 * Service Operation - DROP_USER_ITEM
	 *
	 * @param in_defId       The unique id of the item definition to drop.
	 * @param in_quantity    The quantity of the item to drop.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void dropUserItem(const FString &itemId, int quantity, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Returns a list of items on promotion available to the current user.
	 *
	 * Service Name - userItems
	 * Service Operation - GET_ITEMS_ON_PROMOTION
	 *
	 * @param in_shopId                  Store ID.
	 * @param in_includeDef              Include associated item definition if true.
	 * @param in_includePromotionDetails Include promotion details if true.
	 * @param in_optionsJson             JSON string specifying additional options (e.g., category).
	 * @param in_callback                Callback invoked when the server response is received.
	 */
	void getItemsOnPromotion(const FString &shopId, bool includeDef, bool includePromotionDetails, const FString &optionsJson = TEXT(""), IServerCallback *callback = nullptr);

	/**
	 * Returns a list of promotional details for a specified item.
	 *
	 * Service Name - userItems
	 * Service Operation - GET_ITEM_PROMOTION_DETAILS
	 *
	 * @param in_defId                    Item definition ID.
	 * @param in_shopId                   Store ID.
	 * @param in_includeDef               Include associated item definition if true.
	 * @param in_includePromotionDetails  Include promotion details if true.
	 * @param in_callback                 Callback invoked when the server response is received.
	 */
	void getItemPromotionDetails(const FString &defId, const FString &shopId, bool includeDef, bool includePromotionDetails, IServerCallback *callback = nullptr);

	/**
	 * Retrieves a page of the user's inventory.
	 *
	 * Service Name - userItems
	 * Service Operation - GET_USER_INVENTORY_PAGE
	 *
	 * @param in_context     Context string used to filter inventory.
	 * @param in_includeDef  If true, include associated item definitions in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getUserItemsPage(const FString &context, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Retrieves a page of the user's inventory with an offset.
	 *
	 * Service Name - userItems
	 * Service Operation - GET_USER_INVENTORY_PAGE_OFFSET
	 *
	 * @param in_context     Context string used to filter inventory.
	 * @param in_pageOffset  Page offset to retrieve.
	 * @param in_includeDef  If true, include associated item definitions in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getUserItemsPageOffset(const FString &context, int pageOffest, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Retrieves a specific user item.
	 *
	 * Service Name - userItems
	 * Service Operation - GET_USER_ITEM
	 *
	 * @param in_itemId      ID of the user item to retrieve.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getUserItem(const FString &itemId, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Gifts an item to another user.
	 *
	 * Service Name - userItems
	 * Service Operation - GIVE_USER_ITEM_TO
	 *
	 * @param in_profileId   Profile ID of the recipient.
	 * @param in_itemId      ID of the item to gift.
	 * @param in_version     Version of the item being gifted.
	 * @param in_quantity    Quantity of the item to gift.
	 * @param in_immediate   If true, the gift is delivered immediately.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void giveUserItemTo(const FString &profileId, const FString &itemId, int version, int quantity, bool immediate, IServerCallback *callback = nullptr);

	/**
	 * Purchases a user item from a store.
	 *
	 * Service Name - userItems
	 * Service Operation - PURCHASE_USER_ITEM
	 *
	 * @param in_defId       The unique id of the item definition to purchase.
	 * @param in_quantity    Quantity of the item to purchase.
	 * @param in_shopId      Store ID for the purchase.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void purchaseUserItem(const FString &defId, int quantity, const FString &shopId, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Purchases a user item with options.
	 *
	 * Service Name - userItems
	 * Service Operation - PURCHASE_USER_ITEM
	 *
	 * @param in_defId       The unique id of the item definition to purchase.
	 * @param in_quantity    Quantity of the item to purchase.
	 * @param in_shopId      Store ID for the purchase.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_optionsJson JSON string specifying additional options.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void purchaseUserItemsWithOptions(
		const FString &defId,
		int quantity,
		const FString &shopId,
		bool includeDef,
		const FString &optionsJson,
		IServerCallback *callback = nullptr);

	/**
	 * Retrieves and transfers a gift item from another user.
	 *
	 * Service Name - userItems
	 * Service Operation - RECEIVE_USER_ITEM_FROM
	 *
	 * @param in_profileId   Profile ID of the sender.
	 * @param in_itemId      ID of the item being received.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void receiveUserItemFrom(const FString &profileId, const FString &itemId, IServerCallback *callback = nullptr);

	/**
	 * Opens a quantity of a bundle user item.
	 * Creates applicable items and awards any currencies.
	 *
	 * Service Name - userItems
	 * Service Operation - OPEN_BUNDLE
	 *
	 * @param in_itemId      ID of the bundle item to open.
	 * @param in_version     Version of the bundle item (pass -1 for any version).
	 * @param in_quantity    Quantity of the item to open.
	 * @param in_includeDef  Include associated item definitions if true.
	 * @param in_optionsJson JSON string specifying additional options.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void openBundle(const FString &itemId, int version, int quantity, bool includeDef, const FString &optionsJson, IServerCallback *callback = nullptr);

	/**
	 * Sells a user item back to the store.
	 *
	 * Service Name - userItems
	 * Service Operation - SELL_USER_ITEM
	 *
	 * @param in_itemId      ID of the user item to sell.
	 * @param in_version     Version of the item being sold.
	 * @param in_quantity    Quantity of the item to sell.
	 * @param in_shopId      Store ID for the sale.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sellUserItem(const FString &itemId, int version, int quantity, const FString &shopId, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Updates the data of a specific user item.
	 *
	 * Service Name - userItems
	 * Service Operation - UPDATE_USER_ITEM_DATA
	 *
	 * @param in_itemId      ID of the user item to update.
	 * @param in_version     Version of the item being updated.
	 * @param in_newItemData JSON string with updated item data.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateUserItemData(const FString &itemId, int version, const FString &newItemData, IServerCallback *callback = nullptr);

	/**
	 * Uses a user item, potentially consuming it.
	 *
	 * Service Name - userItems
	 * Service Operation - USE_USER_ITEM
	 *
	 * @param in_itemId      ID of the user item to use.
	 * @param in_version     Version of the user item (pass -1 for any version).
	 * @param in_newItemData Optional JSON string to update item fields.
	 * @param in_includeDef  If true, include associated item definition in the response.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void useUserItem(const FString &itemId, int version, const FString &newItemData, bool includeDef, IServerCallback *callback = nullptr);

	/**
	 * Publishes a user item to the blockchain.
	 *
	 * Service Name - userItems
	 * Service Operation - PUBLISH_USER_ITEM_TO_BLOCKCHAIN
	 *
	 * @param in_itemId      ID of the user item to publish.
	 * @param in_version     Version of the item to publish.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void publishUserItemToBlockchain(const FString &itemId, int version, IServerCallback *callback = nullptr);

	/**
	 * Refreshes blockchain user items.
	 *
	 * Service Name - userItems
	 * Service Operation - REFRESH_BLOCKCHAIN_USER_ITEMS
	 *
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void refreshBlockchainUserItems(IServerCallback *callback = nullptr);

	/**
	 * Removes a user item from the blockchain.
	 *
	 * Service Name - userItems
	 * Service Operation - REMOVE_USER_ITEM_FROM_BLOCKCHAIN
	 *
	 * @param in_itemId   ID of the user item to remove.
	 * @param in_version  Version of the user item to remove.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void removeUserItemFromBlockchain(const FString &itemId, int version, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
};
