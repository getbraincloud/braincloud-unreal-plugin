// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;
class IAcl;

class BCCLIENTPLUGIN_API BrainCloudEntity
{
public:
	BrainCloudEntity(BrainCloudClient *client);

	/**
	 * Method creates a new entity on the server.
	 *
	 * Service Name - entity
	 * Service Operation - CREATE
	 *
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData The entity's data as a json string
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param callback The method to be invoked when the server response is received
	 */
	void createEntity(const FString &entityType, const FString &jsonEntityData, IAcl *jsonEntityAcl, IServerCallback *callback);

	/**
	 * Method updates a new entity on the server. This operation results in the entity
	 * data being completely replaced by the passed in JSON string.
	 *
	 * Service Name - entity
	 * Service Operation - UPDATE
	 *
	 * @param entityId The id of the entity to update
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData The entity's data as a json string.
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param version Current version of the entity. If the version of the
	 * entity on the server does not match the version passed in, the
	 * server operation will fail. Use -1 to skip version checking.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntity(const FString &entityId, const FString &entityType, const FString &jsonEntityData, IAcl *jsonEntityAcl, int32 in_version, IServerCallback *callback);

	/** Method to get a specific entity.
	 *
	 * Service Name - entity
	 * Service Operation - READ
	 *
	 * @param entityId The entity id
	 * @param callback The method to be invoked when the server response is received
	 */
	void getEntity(const FString &entityId, IServerCallback *callback);

	/**
	 * Method retreives a singleton entity on the server. If the entity doesn't exist, null is returned.
	 *
	 * Service Name - entity
	 * Service Operation - READ_SINGLETON
	 *
	 * @param entityType The entity type as defined by the user
	 * @param callback The method to be invoked when the server response is received
	 */
	void getSingleton(const FString &entityType, IServerCallback *callback);

	/**
	 * Method updates a new singleton entity on the server. This operation results in the entity
	 * data being completely replaced by the passed in JSON string. If the entity doesn't exists it is created
	 *
	 * Service Name - entity
	 * Service Operation - UPDATE_SINGLETON
	 *
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData  The entity's data as a json string.
	 * permissions which make the entity readable/writeable by only the user.
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param version Current version of the entity. If the version of the
	 * entity on the server does not match the version passed in, the
	 * server operation will fail. Use -1 to skip version checking.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateSingleton(const FString &entityType, const FString &jsonEntityData, IServerCallback *callback);

	/**
	 * Method deletes the given entity on the server.
	 *
	 * Service Name - entity
	 * Service Operation - DELETE
	 *
	 * @param entityId The id of the entity to update
	 * @param version Current version of the entity. If the version of the
	 * entity on the server does not match the version passed in, the
	 * server operation will fail. Use -1 to skip version checking.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteEntity(const FString &entityId, IServerCallback *callback);

	/**
	 * Method deletes the given singleton entity on the server.
	 *
	 * Service Name - entity
	 * Service Operation - DELETE_SINGLETON
	 *
	 * @param entityType The type of the entity to delete
	 * @param version Current version of the entity. If the version of the
	 * entity on the server does not match the version passed in, the
	 * server operation will fail. Use -1 to skip version checking.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteSingleton(const FString &entityType, IServerCallback *callback);

	/**
	 * Method returns all user entities that match the given type.
	 * Service Name - entity
	 * Service Operation - READ_BY_TYPE
	 *
	 * @param entityType The entity type to search for
	 * @param callback The method to be invoked when the server response is received
	 */
	void getEntitiesByType(const FString &entityType, IServerCallback *callback);

	/**
	 * Method returns a shared entity for the given user and entity ID.
	 * An entity is shared if its ACL allows for the currently logged
	 * in user to read the data.
	 *
	 * Service Name - entity
	 * Service Operation - READ_SHARED_ENTITY
	 *
	 * @param profileId The the profile ID of the user who owns the entity
	 * @param entityId The ID of the entity that will be retrieved
	 * @param callback The method to be invoked when the server response is received
	 */
	void getSharedEntityForProfileId(const FString &profileId, const FString &entityId, IServerCallback *callback = nullptr);

	/**
	 * Method returns all shared entities for the given profile id.
	 * An entity is shared if its ACL allows for the currently logged
	 * in user to read the data.
	 *
	 * Service Name - entity
	 * Service Operation - READ_SHARED
	 *
	 * @param profileId The profile id to retrieve shared entities for
	 * @param callback The method to be invoked when the server response is received
	 */
	void getSharedEntitiesForProfileId(const FString &profileId, IServerCallback *callback);

	/**
	 * Method gets list of shared entities for the specified user based on type and/or where clause
	 *
	 * Service Name - entity
	 * Service Operation - READ_SHARED_ENTITIES_LIST
	 *
	 * @param profileId The profile ID to retrieve shared entities for
	 * @param whereJson Mongo style query
	 * @param orderByJson Sort order
	 * @param maxReturn The maximum number of entities to return
	 * @param callback The method to be invoked when the server response is received
	 */
	void getSharedEntitiesListForProfileId(const FString &profileId, const FString &whereJson, const FString &orderByJson, int32 maxReturn, IServerCallback *callback = nullptr);

	/**
	 * Method updates a shared entity owned by another user. This operation results in the entity
	 * data being completely replaced by the passed in JSON string.
	 *
	 * Service Name - entity
	 * Service Operation - UPDATE_SHARED
	 *
	 * @param entityId The id of the entity to update
	 * @param targetProfileId The id of the user who owns the shared entity
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData The entity's data as a json string.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateSharedEntity(const FString &entityId, const FString &targetProfileId, const FString &entityType, const FString &jsonEntityData, IServerCallback *callback);

	/**
	 * Method gets list of entities from the server base on type and/or where clause
	 *
	 * Service Name - entity
	 * Service Operation - GET_LIST
	 *
	 * @param whereJson Mongo style query string
	 * @param orderByJson Sort order
	 * @param maxReturn The maximum number of entities to return
	 * @param callback The method to be invoked when the server response is received
	 */
	void getList(const FString &whereJson, const FString &orderByJson, int32 maxReturn, IServerCallback *callback);

	/**
	 * Method gets a count of entities based on the where clause
	 *
	 * Service Name - entity
	 * Service Operation - GET_LIST_COUNT
	 *
	 * @param whereJson Mongo style query string
	 * @param callback The method to be invoked when the server response is received
	 */
	void getListCount(const FString &whereJson, IServerCallback *callback);

	/**
	 * Method uses a paging system to iterate through user entities
	 * After retrieving a page of entities with this method,
	 * use GetPageOffset() to retrieve previous or next pages.
	 *
	 * Service Name - entity
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
	 * Service Name - entity
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
	 * Partial increment of entity data field items. Partial set of items incremented as specified.
	 *
	 * Service Name - entity
	 * Service Operation - INCREMENT_USER_ENTITY_DATA
	 *
	 * @param entityId The id of the entity to update
	 * @param jsonData The entity's data object
	 * @param callback The method to be invoked when the server response is received
	 */
	void incrementUserEntityData(const FString &entityId, const FString &jsonData, IServerCallback *callback = nullptr);

	/**
	 * Partial increment of entity data field items. Partial set of items incremented as specified.
	 *
	 * Service Name - entity
	 * Service Operation - INCREMENT_SHARED_USER_ENTITY_DATA
	 *
	 * @param entityId The id of the entity to update
	 * @param targetProfileId Profile ID of the entity owner
	 * @param jsonData The entity's data object
	 * @param callback The method to be invoked when the server response is received
	 */
	void incrementSharedUserEntityData(const FString &entityId, const FString &targetProfileId, const FString &jsonData, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
};
