// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCAuthType.h"
#include "BrainCloudFriend.generated.h"

class BrainCloudClient;
class IServerCallback;

UENUM(BlueprintType)
enum class EFriendPlatform : uint8
{
	ALL UMETA(DisplayName = "All"),
	BRAINCLOUD UMETA(DisplayName = "brainCloud"),
	FACEBOOK UMETA(DisplayName = "Facebook")
};

class BCCLIENTPLUGIN_API BrainCloudFriend
{
public:
	BrainCloudFriend(BrainCloudClient *client);

	/**
	 * Retrieves profile information for the specified user.
	 *
	 * Service Name - friend
	 * Service Operation - GET_PROFILE_INFO_FOR_CREDENTIAL
	 *
	 * @param in_externalId The users's external ID
	 * @param in_authenticationType The authentication type of the user ID
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getProfileInfoForCredential(const FString &externalId, EBCAuthType authenticationType, IServerCallback *callback = nullptr);

	/**
		* Retrieves profile information for the specified user.
		Silently fails, if profile does not exist, just returns null and success, instead of an error.
		*
		* Service Name - friend
		* Service Operation - GET_PROFILE_INFO_FOR_CREDENTIAL_IF_EXISTS
		*
		* @param in_externalId The users's external ID
		* @param in_authenticationType The authentication type of the user ID
		* @param in_callback Method to be invoked when the server response is received.
		*/
	void getProfileInfoForCredentialIfExists(const FString &externalId, EBCAuthType authenticationType, IServerCallback *callback = nullptr);

	/**
	 * Retrieves profile information for the specified external auth user.
	 *
	 * Service Name - friend
	 * Service Operation - GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID
	 *
	 * @param in_externalId External ID of the friend to find
	 * @param in_externalAuthType The external authentication type used for this friend's external ID
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getProfileInfoForExternalAuthId(const FString &externalId, const FString &externalAuthType, IServerCallback *callback = nullptr);

	/**
	 * Retrieves profile information for the specified user. Silently fails, if profile does not exist, just returns null and success, instead of an error.
	 *
	 * Service Name - Friend
	 * Service Operation - GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID_IF_EXISTS
	 *
	 * @param in_externalId External ID of the friend to find
	 * @param in_externalAuthType The external authentication type used for this friend's external ID
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getProfileInfoForExternalAuthIdIfExists(const FString &externalId, const FString &externalAuthType, IServerCallback *callback = nullptr);

	/**
	 * Retrieves the external ID for the specified user profile ID on the specified social platform.
	 *
	 * @param in_profileId Profile (user) ID.
	 * @param in_authenticationType Associated authentication type.
	 */
	void getExternalIdForProfileId(const FString &profileId, const FString &authenticationType, IServerCallback *callback = nullptr);

	/**
	 * Returns a particular entity of a particular friend.
	 *
	 * Service Name - Friend
	 * Service Operation - ReadFriendEntity
	 *
	 * @param in_entityId Id of entity to retrieve.
	 * @param in_friendId Profile Id of friend who owns entity.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void readFriendEntity(const FString &entityId, const FString &friendId, IServerCallback *callback = nullptr);

	/**
	 * Returns entities of all friends optionally based on type.
	 *
	 * Service Name - Friend
	 * Service Operation - ReadFriendsEntities
	 *
	 * @param in_entityType Types of entities to retrieve.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void readFriendsEntities(const FString &entityType, IServerCallback *callback = nullptr);

	/**
	 * Read a friend's user state.
	 * If you are not friend with this user, you will get an error
	 * with NOT_FRIENDS reason code.
	 *
	 * Service Name - Friend
	 * Service Operation - ReadFriendsPlayerState
	 *
	 * @param in_friendId Target friend
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void readFriendUserState(const FString &friendId, IServerCallback *callback = nullptr);

	/**
	 * Returns user state of a particular user.
	 *
	 * Service Name - Friend
	 * Service Operation - GET_SUMMARY_DATA_FOR_PROFILE_ID
	 *
	 * @param in_profileId Profile Id of user to retrieve user state for.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getSummaryDataForProfileId(const FString &profileId, IServerCallback *callback = nullptr);

	/**
	 * Finds a list of users matching the search text by performing an exact match search
	 *
	 * Service Name - friend
	 * Service Operation - FIND_USERS_BY_EXACT_NAME
	 *
	 * @param searchText The string to search for.
	 * @param maxResults  Maximum number of results to return.
	 * @param callback Method to be invoked when the server response is received.
	 */
	void findUsersByExactName(const FString &searchText, int32 maxResults, IServerCallback *callback = nullptr);

	/**
	 * Finds a list of users matching the search text by performing a substring
	 * search of all user names.
	 *
	 * Service Name - friend
	 * Service Operation - FIND_USERS_BY_SUBSTR_NAME
	 *
	 * @param searchText The substring to search for. Minimum length of 3 characters.
	 * @param maxResults  Maximum number of results to return. If there are more the message
	 * @param callback Method to be invoked when the server response is received.
	 */
	void findUsersBySubstrName(const FString &searchText, int32 maxResults, IServerCallback *callback = nullptr);

	/**
	 * Retrieves profile information of the specified universal Id.
	 *
	 * @param in_searchText Universal ID text on which to search.
	 */
	void findUserByExactUniversalId(const FString &searchText, IServerCallback *callback = nullptr);

	/**
	 * Retrieves profile information for the users whos UniversalId start with search text.
	 *
	 * @param in_searchText Universal ID text on which to search.
	 * @param in_maxResults Maximum number of results to return.
	 */
	void findUsersByUniversalIdStartingWith(const FString &searchText, int32 maxResults, IServerCallback *callback = nullptr);

	/**
	 * Retrieves profile information for the users whos names start with search text.
	 *
	 * @param in_searchText Name text on which to search.
	 * @param in_maxResults Maximum number of results to return.
	 */
	void findUsersByNameStartingWith(const FString &searchText, int32 maxResults, IServerCallback *callback = nullptr);

	/**
	 * Retrieves a list of user and friend platform information for all friends of the current user.
	 *
	 * Service Name - Friend
	 * Service Operation - LIST_FRIENDS
	 *
	 * @param in_friendPlatform Friend platform to query.
	 * @param in_includeSummaryData  True if including summary data; false otherwise.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void listFriends(EFriendPlatform friendPlatform, bool includeSummaryData, IServerCallback *callback = nullptr);

	/**
	 * Retrieves the social information associated with the logged in user. Includes summary data if includeSummaryData is true.
	 *
	 * Service Name - Friend
	 * Service Operation - GET_MY_SOCIAL_INFO
	 *
	 * @param friendPlatform Friend platform to query.
	 * @param includeSummaryData  True if including summary data; false otherwise.
	 * @param callback Method to be invoked when the server response is received.
	 */
	void getMySocialInfo(EFriendPlatform friendPlatform, bool includeSummaryData, IServerCallback *callback = nullptr);

	/**
	 * Links the current user and the specified users as brainCloud friends.
	 *
	 * Service Name - Friend
	 * Service Operation - ADD_FRIENDS
	 *
	 * @param in_profileIds Collection of profile IDs.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void addFriends(const TArray<FString> &profileIds, IServerCallback *callback = nullptr);

	/**
	 * Links the profiles for the specified externalIds for the given friend platform as internal friends.
	 *
	 * Service Name - Friend
	 * Service Operation - ADD_FRIENDS_FROM_PLATFORM
	 *
	 * @param in_friendPlatform Platform to add from (i.e: FriendPlatform::Facebook)
	 * @param in_mode ADD or SYNC
	 * @param in_externalIds Collection of external IDs from the friend platform.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void addFriendsFromPlatform(const EFriendPlatform &friendPlatform, FString mode, const TArray<FString> &externalIds, IServerCallback *callback = nullptr);

	/**
	 * Unlinks the current user and the specified users as brainCloud friends.
	 *
	 * Service Name - Friend
	 * Service Operation - REMOVE_FRIENDS
	 *
	 * @param in_profileIds Collection of profile IDs.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void removeFriends(const TArray<FString> &profileIds, IServerCallback *callback = nullptr);

	/**
	 * Get users online status
	 *
	 * Service Name - Friend
	 * Service Operation - GET_USERS_ONLINE_STATUS
	 *
	 * @param in_profileIds Collection of profile IDs.
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getUsersOnlineStatus(const TArray<FString> &profileIds, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;

	TMap<EFriendPlatform, FString> _platformStrings;
};
