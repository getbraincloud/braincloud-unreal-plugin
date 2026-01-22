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
	 * Service Name - CustomEntity
	 * Service Operation - CreateEntity
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_jsonEntityData The entity's data as a json string
	 * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param timeToLive
	 * @param isOwned
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void createEntity(const FString &entityType, const FString &jsonEntityData, IAcl *jsonEntityAcl, int64 timeToLive, bool isOwned, IServerCallback *callback);

	/**
	 * Deletes the specified custom entity on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - DeleteEntity
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_jsonEntityData The entity's data as a json string
	 * @param version
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteEntity(const FString &entityType, const FString &entityId, int version, IServerCallback *callback);

	/**
	 * Deletes the specified custom entity on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - GetCount
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_whereJson
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getCount(const FString &entityType, const FString &whereJson, IServerCallback *callback);

	/**
	 * Method uses a paging system to iterate through Custom Entities
	 * After retrieving a page of Custom Entities with this method,
	 * use GetEntityPageOffset() to retrieve previous or next pages.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - GetCustomEntityPage
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_context The json context for the page request.
	 *                   See the portal appendix documentation for format.
	 * @param in_callback The callback object
	 */
	void getEntityPage(const FString &entityType, const FString &context, IServerCallback *callback);

	/**
	 * Gets the page of custom entities from the server based on the encoded context and specified page offset.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - GetEntityPageOffset
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_context
	 * @param in_pageOffset
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getEntityPageOffset(const FString &entityType, const FString &context, int pageOffset, IServerCallback *callback);

	/**
	 * Reads the specified custom entity from the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - ReadEntity
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_entityId The entity id as defined by the system
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void readEntity(const FString &entityType, const FString &entityId, IServerCallback *callback);

	/**
	 * Increments fields on the specified custom entity owned by the user on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - IncrementData
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_entityId The entity id as defined by the system
	 * @param in_fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void incrementData(const FString &entityType, const FString &entityId, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * Increments the specified fields, of the singleton owned by the user, by the specified amount within the custom entity data on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - IncrementSingletonData
	 *
	 * @param in_entityType The type of custom entity being updated.
	 * @param in_fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void incrementSingletonData(const FString &entityType, const FString &fieldsJson, IServerCallback *callback);

	/**
	 *
	 * Service Name - CustomEntity
	 * Service Operation - GetRandomEntitiesMatching
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_whereJson
	 * @param in_maxReturn
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getRandomEntitiesMatching(const FString &entityType, const FString &whereJson, const int64 &maxReturn, IServerCallback *callback);

	/**
	 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - UpdateEntity
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_entityId
	 * @param in_version
	 * @param in_jsonEntityData The entity's data as a json string
	 * @param in_jsonEntityAcl The entity's access control list as json. A null acl implies default
	 * permissions which make the entity readable/writeable by only the user.
	 * @param timeToLive
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateEntity(const FString &entityType, const FString &entityId, int version, const FString &dataJson, IAcl *jsonEntityAcl, int64 timeToLive, IServerCallback *callback);

	/**
	 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - UpdateEntityFields
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_entityId
	 * @param in_version
	 * @param in_fieldsJson
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateEntityFields(const FString &entityType, const FString &entityId, int version, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * For sharded custom collection entities. Sets the specified fields within custom entity data on the server, enforcing ownership/ACL permissions.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - UpdateEntityFieldsSharded
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_entityId
	 * @param in_version
	 * @param in_fieldsJson
	 * @param in_shardKeyJson The shard key field(s) and value(s), as JSON, applicable to the entity being updated.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateEntityFieldsSharded(const FString &entityType, const FString &entityId, int version, const FString &fieldsJson, const FString &shardKeyJson, IServerCallback *callback);

	/**
	 * deletes entities based on the delete criteria.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - DeleteEntities
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_deleteCriteria Json string of criteria wanted for deletion
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteEntities(const FString &entityType, const FString &deleteCriteria, IServerCallback *callback);

	/**
	 * Reads the custom entity singleton owned by the session's user.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - ReadSingleton
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void readSingleton(const FString &entityType, IServerCallback *callback);

	/**
	 * Updates the singleton owned by the user for the specified custom entity type on the server,
	 * creating the singleton if it does not exist.
	 * This operation results in the owned singleton's data being completely replaced by the passed in JSON object.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - UpdateSingleton
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_version
	 * @param in_dataJson
	 * @param in_acl
	 * @param in_timeToLive
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateSingleton(const FString &entityType, int version, const FString &dataJson, IAcl *jsonEntityAcl, int64 timeToLive, IServerCallback *callback);

	/**
	 * Partially updates the data, of the singleton owned by the user for the specified custom entity type,
	 * with the specified fields, on the server
	 *
	 * Service Name - CustomEntity
	 * Service Operation - UpdateSingletonFields
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_version
	 * @param in_fieldsJson
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateSingletonFields(const FString &entityType, int version, const FString &fieldsJson, IServerCallback *callback);

	/**
	 * Deletes the specified custom entity singleton, owned by the session's user,
	 * for the specified entity type, on the server.
	 *
	 * Service Name - CustomEntity
	 * Service Operation - DeleteSingleton
	 *
	 * @param in_entityType The entity type as defined by the user
	 * @param in_version
	 */
	void deleteSingleton(const FString &entityType, int version, IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
