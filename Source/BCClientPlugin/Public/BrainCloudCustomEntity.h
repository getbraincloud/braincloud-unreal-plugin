// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;
class IAcl;

class BCCLIENTPLUGIN_API BrainCloudCustomEntity
{
public:
	BrainCloudCustomEntity(BrainCloudClient *client);

	/**
	 * Creates new custom entity.
	 *
	 * Service Name - customEntity
	 * Service Operation - CREATE_ENTITY
	 *
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData The entity's data as a json string
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param isOwned
	 * @param callback The method to be invoked when the server response is received
	 */
	void createEntity(const FString &entityType, const FString &jsonEntityData, IAcl *jsonEntityAcl, int64 timeToLive, bool isOwned, IServerCallback *callback);

	/**
	 * Deletes the specified custom entity on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - DELETE_ENTITY
	 *
	 * @param entityType The entity type as defined by the user
	 * @param jsonEntityData The entity's data as a json string
	 * @param version Version of the custom entity being updated.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteEntity(const FString &entityType, const FString &entityId, int version, IServerCallback *callback);

	/**
	 * Counts the number of custom entities meeting the specified where clause, enforcing ownership/ACL permissions
	 *
	 * Service Name - customEntity
	 * Service Operation - GET_COUNT
	 *
	 * @param entityType The entity type as defined by the user
	 * @param whereJson Mongo style query string
	 * @param callback The method to be invoked when the server response is received
	 */
	void getCount(const FString &entityType, const FString &whereJson, IServerCallback *callback);

	/**
	 * Method uses a paging system to iterate through Custom Entities
	 * After retrieving a page of Custom Entities with this method,
	 * use GetEntityPageOffset() to retrieve previous or next pages.
	 *
	 * Service Name - customEntity
	 * Service Operation - GET_ENTITY_PAGE
	 *
	 * @param entityType The entity type as defined by the user
	 * @param context The json context for the page request.
	 *                   See the portal appendix documentation for format.
	 * @param callback The callback object
	 */
	void getEntityPage(const FString &entityType, const FString &context, IServerCallback *callback);

	/**
	 * Gets the page of custom entities from the server based on the encoded context and specified page offset.
	 *
	 * Service Name - customEntity
	 * Service Operation - GET_ENTITY_PAGE_OFFSET
	 *
	 * @param entityType The entity type as defined by the user
	 * @param context The context string returned from the server from a previous
	 *                   call to GetPage or GetPageOffset.
	 * @param pageOffset The positive or negative page offset to fetch. Uses the
	 *                   last page retrieved using the context string to determine a
	 *                   starting point.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getEntityPageOffset(const FString &entityType, const FString &context, int pageOffset, IServerCallback *callback);

	/**
	 * Reads the specified custom entity from the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - READ_ENTITY
	 *
	 * @param entityType The entity type as defined by the user
	 * @param entityId The entity id as defined by the system
	 * @param callback The method to be invoked when the server response is received
	 */
	void readEntity(const FString &entityType, const FString &entityId, IServerCallback *callback);

	/**
	 * Increments fields on the specified custom entity owned by the user on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - INCREMENT_DATA
	 *
	 * @param entityType The entity type as defined by the user
	 * @param entityId The entity id as defined by the system
	 * @param fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
	 * @param callback The method to be invoked when the server response is received
	 */
	void incrementData(const FString &entityType, const FString &entityId, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * Increments the specified fields, of the singleton owned by the user, by the specified amount within the custom entity data on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - INCREMENT_SINGLETON_DATA
	 *
	 * @param entityType The type of custom entity being updated.
	 * @param fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
	 * @param callback The method to be invoked when the server response is received
	 */
	void incrementSingletonData(const FString &entityType, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * Gets a list of up to maxReturn randomly selected custom entities from the
	 * server based on the entity type and where condition.
	 *
	 * Service Name - customEntity
	 * Service Operation - GET_RANDOM_ENTITIES_MATCHING
	 *
	 * @param entityType The entity type as defined by the user
	 * @param whereJson Mongo style query string
	 * @param maxReturn Max number of returns
	 * @param callback The method to be invoked when the server response is received
	 */
	void getRandomEntitiesMatching(const FString &entityType, const FString &whereJson, const int64 &maxReturn, IServerCallback *callback);

	/**
	 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - UPDATE_ENTITY
	 *
	 * @param entityType The entity type as defined by the user
	 * @param entityId The id of custom entity being updated.
	 * @param version Version of the custom entity being updated.
	 * @param jsonEntityData The entity's data as a json string
	 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntity(const FString &entityType, const FString &entityId, int version, const FString &dataJson, IAcl *jsonEntityAcl, int64 timeToLive, IServerCallback *callback);

	/**
	 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - UPDATE_ENTITY_FIELDS
	 *
	 * @param entityType The entity type as defined by the user
	 * @param entityId The id of custom entity being updated.
	 * @param version Version of the custom entity being updated.
	 * @param fieldsJson Specific fields, as JSON, to set within entity's custom data.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityFields(const FString &entityType, const FString &entityId, int version, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * For sharded custom collection entities. Sets the specified fields within custom entity data on the server, enforcing ownership/ACL permissions.
	 *
	 * Service Name - customEntity
	 * Service Operation - UPDATE_ENTITY_FIELDS_SHARDED
	 *
	 * @param entityType The entity type as defined by the user
	 * @param entityId The id of custom entity being updated.
	 * @param version Version of the custom entity being updated.
	 * @param fieldsJson Specific fields, as JSON, to set within entity's custom data.
	 * @param shardKeyJson The shard key field(s) and value(s), as JSON, applicable to the entity being updated.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateEntityFieldsSharded(const FString &entityType, const FString &entityId, int version, const FString &fieldsJson, const FString &shardKeyJson, IServerCallback *callback);

	/**
	 * deletes entities based on the delete criteria.
	 *
	 * Service Name - customEntity
	 * Service Operation - DELETE_ENTITIES
	 *
	 * @param entityType The entity type as defined by the user
	 * @param deleteCriteria Json string of criteria wanted for deletion
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteEntities(const FString &entityType, const FString &deleteCriteria, IServerCallback *callback);

	/**
	 * Reads the custom entity singleton owned by the session's user.
	 *
	 * Service Name - customEntity
	 * Service Operation - READ_SINGLETON
	 *
	 * @param entityType The entity type as defined by the user
	 * @param callback The method to be invoked when the server response is received
	 */
	void readSingleton(const FString &entityType, IServerCallback *callback);

	/**
	 * Updates the singleton owned by the user for the specified custom entity type on the server,
	 * creating the singleton if it does not exist.
	 * This operation results in the owned singleton's data being completely replaced by the passed in JSON object.
	 *
	 * Service Name - customEntity
	 * Service Operation - UPDATE_SINGLETON
	 *
	 * @param entityType The entity type as defined by the user
	 * @param version Version of the singleton being updated.
	 * @param dataJson The full data for the singleton as a json string
	 * @param acl The singleton entity's Access Control List as an object.
	 * 				 A null ACL implies default permissions which make the entity readable by others.
	 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
	 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateSingleton(const FString &entityType, int version, const FString &dataJson, IAcl *jsonEntityAcl, int64 timeToLive, IServerCallback *callback);

	/**
	 * Partially updates the data, of the singleton owned by the user for the specified custom entity type,
	 * with the specified fields, on the server
	 *
	 * Service Name - customEntity
	 * Service Operation - UPDATE_SINGLETON_FIELDS
	 *
	 * @param entityType The entity type as defined by the user
	 * @param version Version of the singleton being updated.
	 * @param fieldsJson Specific fields, as JSON, within entity's custom data to be updated.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateSingletonFields(const FString &entityType, int version, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * Deletes the specified custom entity singleton, owned by the session's user,
	 * for the specified entity type, on the server.
	 *
	 * Service Name - customEntity
	 * Service Operation - DELETE_SINGLETON
	 *
	 * @param entityType The entity type as defined by the user
	 * @param version Version of the singleton being deleted.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteSingleton(const FString &entityType, int version, IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
