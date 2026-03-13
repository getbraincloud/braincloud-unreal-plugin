// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;
class IAcl;

class BCCLIENTPLUGIN_API BrainCloudGlobalEntity
{
public:
	BrainCloudGlobalEntity(BrainCloudClient *client);

	/**
	 * Method creates a new entity on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - CREATE
	 *
	 * @param entityType The entity type as defined by the user
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * @param jsonEntityData  The entity's data as a json string
	 * @param callback The method to be invoked when the server response is received
	 */
	void createEntity(const FString &entityType, int64 timeToLive, IAcl *jsonEntityAcl,
					  const FString &jsonEntityData, IServerCallback *callback);

	/**
	 * Method creates a new entity on the server with an indexed id.
	 *
	 * Service Name - globalEntity
	 * Service Operation - CREATE_WITH_INDEXED_ID
	 *
	 * @param entityType The entity type as defined by the user
	 * @param indexedId A secondary ID that will be indexed
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * @param jsonEntityData  The entity's data as a json string
	 * @param callback The method to be invoked when the server response is received
	 */
	void createEntityWithIndexedId(const FString &entityType, const FString &indexedId, int64 timeToLive,
								   IAcl *jsonEntityAcl, const FString &jsonEntityData, IServerCallback *callback);

	/**
	 * Method updates an existing entity on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - UPDATE
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param jsonEntityData  The entity's data as a json string
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntity(const FString &entityId, int32 version, const FString &jsonEntityData, IServerCallback *callback);

	/**
	 * Method updates an existing entity's Acl on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - UPDATE_ACL
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param jsonEntityAcl The entity's access control list as json.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityAcl(const FString &entityId, int32 version, IAcl *jsonEntityAcl, IServerCallback *callback);

	/**
	 * Method updates an existing entity's time to live on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - UPDATE_TIME_TO_LIVE
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityTimeToLive(const FString &entityId, int32 version, int64 timeToLive, IServerCallback *callback);

	/**
	 * Method deletes an existing entity on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - DELETE
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to delete
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteEntity(const FString &entityId, int32 version, IServerCallback *callback);

	/**
	 * Method reads an existing entity from the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - READ
	 *
	 * @param entityId The entity ID
	 * @param callback The method to be invoked when the server response is received
	 */
	void readEntity(const FString &entityId, IServerCallback *callback);

	/**
	 * Method gets list of entities from the server base on type and/or where clause
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_LIST
	 *
	 * @param where Mongo style query string
	 * @param orderBy Sort order
	 * @param maxReturn The maximum number of entities to return
	 * @param callback The method to be invoked when the server response is received
	 */
	void getList(const FString &where, const FString &orderBy, int32 maxReturn, IServerCallback *callback);

	/**
	 * Method gets list of entities from the server base on indexed id
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_LIST_BY_INDEXED_ID
	 *
	 * @param entityIndexedId The entity indexed Id
	 * @param maxReturn The maximum number of entities to return
	 * @param callback The method to be invoked when the server response is received
	 */
	void getListByIndexedId(const FString &entityIndexedId, int32 maxReturn, IServerCallback *callback);

	/**
	 * Method gets a count of entities based on the where clause
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_LIST_COUNT
	 *
	 * @param where Mongo style query string
	 * @param callback The method to be invoked when the server response is received
	 */
	void getListCount(const FString &where, IServerCallback *callback);

	/**
	 * Method uses a paging system to iterate through Global Entities
	 * After retrieving a page of Global Entities with this method,
	 * use GetPageOffset() to retrieve previous or next pages.
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_PAGE
	 *
	 * @param context The json context for the page request.
	 *                   See the portal appendix documentation for format.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getPage(const FString &context, IServerCallback *callback);

	/**
	 * Method to retrieve previous or next pages after having called the GetPage method.
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_PAGE_OFFSET
	 *
	 * @param context The context string returned from the server from a
	 *      previous call to GetPage or GetPageOffset
	 * @param pageOffset The positive or negative page offset to fetch. Uses the last page
	 *      retrieved using the context string to determine a starting point.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getPageOffset(const FString &context, int32 pageOffset, IServerCallback *callback);

	/**
	 * Partial increment of global entity data field items. Partial set of items incremented as specified.
	 *
	 * Service Name - globalEntity
	 * Service Operation - INCREMENT_GLOBAL_ENTITY_DATA
	 *
	 * @param entityId The id of the entity to update
	 * @param jsonData The entity's data object
	 * @param callback The method to be invoked when the server response is received
	 */
	void incrementGlobalEntityData(const FString &entityId, const FString &jsonData, IServerCallback *callback = nullptr);

	/**
	 * Gets a list of up to randomCount randomly selected entities from the server based on the where condition and specified maximum return count.
	 *
	 * Service Name - globalEntity
	 * Service Operation - GET_RANDOM_ENTITIES_MATCHING
	 *
	 * @param where Mongo style query string
	 * @param maxReturn The maximum number of entities to return
	 * @param callback The method to be invoked when the server response is received
	 */
	void getRandomEntitiesMatching(const FString &where, int32 maxReturn, IServerCallback *callback = nullptr);

	/**
	 * Method updates an existing entity's Indexed Id
	 *
	 * Service Name - globalEntity
	 * Service Operation - UPDATE_ENTITY_OWNER_AND_ACL
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param entityIndexedId the id index of the entity
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityIndexedId(const FString &entityId, int32 version, const FString &entityIndexedId, IServerCallback *callback = nullptr);

	/**
	 * Method updates an existing entity's Owner and ACL on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - UPDATE_ENTITY_OWNER_AND_ACL
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param ownerId The owner ID
	 * @param jsonEntityAcl The entity's access control list as JSON.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityOwnerAndAcl(const FString &entityId, int32 version, const FString &ownerId, IAcl *jsonEntityAcl, IServerCallback *callback);

	/**
	 * Method clears the owner id of an existing entity and sets the ACL on the server.
	 *
	 * Service Name - globalEntity
	 * Service Operation - MAKE_SYSTEM_ENTITY
	 *
	 * @param entityId The entity ID
	 * @param version The version of the entity to update
	 * @param jsonEntityAcl The entity's access control list as JSON.
	 * @param callback The method to be invoked when the server response is received
	 */
	void makeSystemEntity(const FString &entityId, int32 version, IAcl *jsonEntityAcl, IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
