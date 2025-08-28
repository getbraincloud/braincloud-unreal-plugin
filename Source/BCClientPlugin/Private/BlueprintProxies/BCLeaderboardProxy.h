// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintCallProxyBase.h"
#include "IServerCallback.h"
#include "BrainCloudLeaderboard.h"
#include "BCLeaderboardProxy.generated.h"

UCLASS(MinimalAPI)
class UBCLeaderboardProxy : public UBCBlueprintCallProxyBase
{
	GENERATED_BODY()

  public:
	UBCLeaderboardProxy(const FObjectInitializer &ObjectInitializer);

	/**
	* Method returns the social leaderboard. A player's social leaderboard is
	* comprised of players who are recognized as being your friend.
	* For now, this applies solely to Facebook connected players who are
	* friends with the logged in player (who also must be Facebook connected).
	* In the future this will expand to other identification means (such as
	* Game Centre, Google circles etc).
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score. The currently logged in player will also
	* be returned in the social leaderboard.
	*
	* Note: If no friends have played the game, the bestScore, createdAt, updatedAt
	* will contain NULL.
	*
	* Param - leaderboardId The id of the leaderboard to retrieve
	* Param - replaceName If true, the currently logged in player's name will be replaced
	* by the string "You".
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetSocialLeaderboard(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, bool replaceName);

	/**
	* Method returns the social leaderboard. A player's social leaderboard is
	* comprised of players who are recognized as being your friend.
	* For now, this applies solely to Facebook connected players who are
	* friends with the logged in player (who also must be Facebook connected).
	* In the future this will expand to other identification means (such as
	* Game Centre, Google circles etc).
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score. The currently logged in player will also
	* be returned in the social leaderboard.
	*
	* Note: If no friends have played the game, the bestScore, createdAt, updatedAt
	* will contain NULL.
	* 
	* This method returns the same data as GetSocialLeaderboard, but it will not return an error if the leaderboard is not found.
	*
	* Param - leaderboardId The id of the leaderboard to retrieve
	* Param - replaceName If true, the currently logged in player's name will be replaced
	* by the string "You".
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetSocialLeaderboardIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, bool replaceName);

	/**
	* Method returns the social leaderboard by its version. A player's social leaderboard is
	* comprised of players who are recognized as being your friend.
	* For now, this applies solely to Facebook connected players who are
	* friends with the logged in player (who also must be Facebook connected).
	* In the future this will expand to other identification means (such as
	* Game Centre, Google circles etc).
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score. The currently logged in player will also
	* be returned in the social leaderboard.
	*
	* Note: If no friends have played the game, the bestScore, createdAt, updatedAt
	* will contain NULL.
	*
	* Param - leaderboardId The id of the leaderboard to retrieve
	* Param - replaceName If true, the currently logged in player's name will be replaced
	* Param - versionId the version
	* by the string "You".
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetSocialLeaderboardByVersion(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, bool replaceName, int32 versionId);

	/**
	* Method returns the social leaderboard by its version. A player's social leaderboard is
	* comprised of players who are recognized as being your friend.
	* For now, this applies solely to Facebook connected players who are
	* friends with the logged in player (who also must be Facebook connected).
	* In the future this will expand to other identification means (such as
	* Game Centre, Google circles etc).
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score. The currently logged in player will also
	* be returned in the social leaderboard.
	*
	* Note: If no friends have played the game, the bestScore, createdAt, updatedAt
	* will contain NULL.
	* 
	* This method returns the same data as GetSocialLeaderboardByVersion, but it will not return an error if the leaderboard does not exist.
	*
	* Param - leaderboardId The id of the leaderboard to retrieve
	* Param - replaceName If true, the currently logged in player's name will be replaced
	* Param - versionId the version
	* by the string "You".
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetSocialLeaderboardByVersionIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, bool replaceName, int32 versionId);

	/**
	* Reads multiple social leaderboards.
	*
	* Param - leaderboardIds Collection of leaderboard IDs.
	* Param - leaderboardResultCount Maximum count of entries to return for each leaderboard.
	* Param - replaceName If true, the currently logged in player's name will be replaced
	* by the string "You".
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetMultiSocialLeaderboard(UBrainCloudWrapper *brainCloudWrapper, const TArray<FString> &leaderboardIds, int32 leaderboardResultCount, bool replaceName);

	/**
	* Method returns a page of global leaderboard results.
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score.
	*
	* Note: This method allows the client to retrieve pages from within the global leaderboard list
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardPage
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - startIndex The index at which to start the page.
	* Param - endIndex The index at which to end the page.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardPage(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 startIndex, int32 endIndex);

	/**
	* Method returns a page of global leaderboard results.
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score.
	*
	* Note: This method allows the client to retrieve pages from within the global leaderboard list
	* 
	* This method returns the same data as GetGlobalLeaderboardPage, but it will not return an error if the leaderboard does not exist.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardPage
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - startIndex The index at which to start the page.
	* Param - endIndex The index at which to end the page.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetGlobalLeaderboardPageIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 startIndex, int32 endIndex);

	/**
	* Method returns a page of global leaderboard results.
	* By using a non-current version id, the user can retrieve a historial leaderboard.
	* See GetGlobalLeaderboardVersions method to retrieve the version id.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardPage
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - startIndex The index at which to start the page.
	* Param - endIndex The index at which to end the page.
	* Param - versionId The historical version to retrieve.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardPageByVersion(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 startIndex, int32 endIndex, int32 versionId);

	/**
	* Method returns a page of global leaderboard results.
	* By using a non-current version id, the user can retrieve a historial leaderboard.
	* See GetGlobalLeaderboardVersions method to retrieve the version id.
	* 
	* This method returns the same data as GetGlobalLeaderboardPageByVersion, but it will not return an error if the leaderboard does not exist.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardPage
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - startIndex The index at which to start the page.
	* Param - endIndex The index at which to end the page.
	* Param - versionId The historical version to retrieve.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetGlobalLeaderboardPageByVersionIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 startIndex, int32 endIndex, int32 versionId);

	/**
	* Method returns a view of global leaderboard results that centers on the current player.
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardView
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - beforeCount The count of number of players before the current player to include.
	* Param - afterCount The count of number of players after the current player to include.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardView(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount);

	/**
	* Method returns a view of global leaderboard results that centers on the current player.
	*
	* Leaderboards entries contain the player's score and optionally, some user-defined
	* data associated with the score.
	* 
	* This method returns the same data as GetGlobalLeaderboardView, but it will not return an error if the leaderboard does not exist.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardView
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - beforeCount The count of number of players before the current player to include.
	* Param - afterCount The count of number of players after the current player to include.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetGlobalLeaderboardViewIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount);

	/**
	* Method returns a view of global leaderboard results that centers on the current player.
	* By using a non-current version id, the user can retrieve a historial leaderboard.
	* See GetGlobalLeaderboardVersions method to retrieve the version id.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardView
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - beforeCount The count of number of players before the current player to include.
	* Param - afterCount The count of number of players after the current player to include.
	* Param - versionId The historical version to retrieve.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardViewByVersion(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount, int32 versionId);

	/**
	* Method returns a view of global leaderboard results that centers on the current player.
	* By using a non-current version id, the user can retrieve a historial leaderboard.
	* See GetGlobalLeaderboardVersions method to retrieve the version id.
	* 
	* This method returns the same data as GetGlobalLeaderboardViewByVersion, but it will not return an error if the leaderboard does not exist.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardViewIfExists
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	* Param - sort Sort key Sort order of page.
	* Param - beforeCount The count of number of players before the current player to include.
	* Param - afterCount The count of number of players after the current player to include.
	* Param - versionId The historical version to retrieve.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetGlobalLeaderboardViewByVersionIfExists(UBrainCloudWrapper* brainCloudWrapper, FString leaderboardId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount, int32 versionId);

	/**
	* Gets the number of entries in a global leaderboard
	*
	* Param - leaderboardId The id of the leaderboard to retrieve.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardEntryCount(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId);

	/** Gets the global leaderboard versions.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - GetGlobalLeaderboardVersions
	*
	* Param - leaderboardId The leaderboard
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGlobalLeaderboardVersions(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId);

	/**
	* Post the players score to the given social leaderboard.
	* You can optionally send a user-defined json string of data
	* with the posted score. This string could include information
	* relevant to the posted score.
	*
	* Note that the behaviour of posting a score can be modified in
	* the brainCloud portal. By default, the server will only keep
	* the player's best score.
	*
	* Param - leaderboardId The leaderboard to post to
	* Param - score The score to post
	* Param - data Optional user-defined data to post with the score
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScore(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, int32 score, FString jsonData);

	/**
	 * @deprecated Please use PostScoreToDynamicLeaderboardUTC instead. Will be removed March 2021
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DeprecatedFunction, DeprecationMessage = "This function has been deprecated. Please use PostScoreToDynamicLeaderboardUTC instead"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicLeaderboard(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, int32 score, FString jsonData,
															  ESocialLeaderboardType leaderboardType, ERotationType rotationType, FDateTime rotationStart, int32 retainedCount);
	
	/**
	* Post the players score to the given social leaderboard.
	* Pass leaderboard config data to dynamically create if necessary.
	* You can optionally send a user-defined json string of data
	* with the posted score. This string could include information
	* relevant to the posted score.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - PostScoreDynamic
	*
	* Param - leaderboardId The leaderboard to post to
	* Param - score The score to post
	* Param - data Optional user-defined data to post with the score
	* Param - leaderboardType leaderboard type
	* Param - rotationType Type of rotation
	* Param - rotationStart Date to start rotation calculations (Date is converted to "dd-mm-yyyy hh:mm" format)
	* Param - retainedCount How many rotations to keep
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicLeaderboardUTC(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, int32 score, FString jsonData,
															  ESocialLeaderboardType leaderboardType, ERotationType rotationType, int64 rotationStart, int32 retainedCount);

	/**
	 * @deprecated Please use PostScoreToDynamicGroupLeaderboardUTC instead. Will be removed March 2021
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DeprecatedFunction, DeprecationMessage = "This function has been deprecated. Please use PostScoreToDynamicGroupLeaderboardUTC instead"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicGroupLeaderboard(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, const FString &groupId, int32 score, FString jsonData,
															  ESocialLeaderboardType leaderboardType, ERotationType rotationType, FDateTime rotationStart, int32 retainedCount);
	
	/**
	* Posts score to group leaderbopard and dynamically creates if necessary.
	* leaderboardType, rotationReset, retainedCount and rotationType are required.
	* uses UTC time in milliseconds since epoch
	*
	* Service Name - SocialLeaderboard
	* Service Operation - POST_GROUP_SCORE_DYNAMIC
	*
	* Param - leaderboardId The leaderboard to post to
	* Param - in_groupId the groups Id
	* Param - score The score to post
	* Param - data Optional user-defined data to post with the score
	* Param - leaderboardType leaderboard type
	* Param - rotationType Type of rotation
	* Param - rotationStart Date to start rotation calculations (Date is converted to "dd-mm-yyyy hh:mm" format)
	* Param - retainedCount How many rotations to keep
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicGroupLeaderboardUTC(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, const FString &groupId, int32 score, FString jsonData,
															  ESocialLeaderboardType leaderboardType, ERotationType rotationType, int64 rotationStart, int32 retainedCount);
	
	/**
	 * @deprecated Please use PostScoreToDynamicLeaderboardDaysUTC instead. Will be removed March 2021
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DeprecatedFunction, DeprecationMessage = "This function has been deprecated. Please use PostScoreToDynamicLeaderboardDaysUTC instead"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicLeaderboardDays(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, int32 score, FString jsonData,
																  ESocialLeaderboardType leaderboardType, FDateTime rotationStart, int32 retainedCount, int32 numDaysToRotate);
	
	/**
	* Post the players score to the given social leaderboard.
	* Pass leaderboard config data to dynamically create if necessary.
	* You can optionally send a user-defined json string of data
	* with the posted score. This string could include information
	* relevant to the posted score.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - PostScoreDynamic
	*
	* Param - leaderboardId The leaderboard to post to
	* Param - score The score to post
	* Param - data Optional user-defined data to post with the score
	* Param - leaderboardType leaderboard type
	* Param - rotationType Type of rotation
	* Param - rotationStart Date to start rotation calculations (Date is converted to "dd-mm-yyyy hh:mm" format)
	* Param - retainedCount How many rotations to keep
	* Param - numDaysToRotate How many days between each rotation
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicLeaderboardDaysUTC(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, int32 score, FString jsonData,
																  ESocialLeaderboardType leaderboardType, int64 rotationStart, int32 retainedCount, int32 numDaysToRotate);

	/**
	* Post the players score to the given social group leaderboard with a
	* rotation type of DAYS. You can optionally send a user-defined
	* JSON string of data with the posted score.
	* This string could include information relevant to the posted score.
	*
	* Service Name - SocialLeaderboard
	* Service Operation - PostScoreDynamic
	*
	* Param leaderboardId The leaderboard to post to
	* Param groupId The ID of the group
	* Param score The score to post
	* Param data Optional user-defined data to post with the score
	* Param leaderboardType leaderboard type
	* Param rotationResetUTC Date to start rotation calculations (Date is converted to "dd-mm-yyyy" format)
	* Param retainedCount How many rotations to keep
	* Param numDaysToRotate How many days between each rotation
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
    static UBCLeaderboardProxy *PostScoreToDynamicGroupLeaderboardDaysUTC(UBrainCloudWrapper *brainCloudWrapper, FString leaderboardId, FString groupId, int32 score, FString jsonData,
                                                                  ESocialLeaderboardType leaderboardType, FDateTime rotationResetUTC, int32 retainedCount, int32 numDaysToRotate);

	/**
	* Removes a player's score from the leaderboard
	*
	* Service Name - leaderboard
	* Service Operation - REMOVE_PLAYER_SCORE
	*
	* Param - leaderboardId The leaderboard ID
	* Param - versionId The version of the leaderboard
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *RemovePlayerScore(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, int32 versionId);

	/**
	* Retrieve the social leaderboard for a group.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - groupId The ID of the group
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGroupSocialLeaderboard(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId);

	/**
	* Retrieve the social leaderboard for a group by its version.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - groupId The ID of the group
	* Param - versionId the version
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGroupSocialLeaderboardByVersion(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId, int32 versionId);

	/**
	* Retrieve the social leaderboard for a list of players.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - profileIds The IDs of the players
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetPlayersSocialLeaderboard(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const TArray<FString> profileIds);

	/**
	* Retrieve the social leaderboard for a list of players.
	* 
	* This function returns the same data as GetPlayersSocialLeaderboard, but it will not return an error if the leaderboard does not exist.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - profileIds The IDs of the players
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetPlayersSocialLeaderboardIfExists(UBrainCloudWrapper* brainCloudWrapper, const FString& leaderboardId, const TArray<FString> profileIds);

	/**
	* Retrieve the social leaderboard for a list of players.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - profileIds The IDs of the players
	* Param - versionId the version
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetPlayersSocialLeaderboardByVersion(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const TArray<FString> profileIds, int32 versionId);

	/**
	* Retrieve the social leaderboard for a list of players.
	* 
	* This function returns the same data as GetPlayersSocialLeaderboardByVersion, but it will not return an error if the leaderboard does not exist.
	*
	* Param - leaderboardId The leaderboard to retrieve
	* Param - profileIds The IDs of the players
	* Param - versionId the version
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* GetPlayersSocialLeaderboardByVersionIfExists(UBrainCloudWrapper* brainCloudWrapper, const FString& leaderboardId, const TArray<FString> profileIds, int32 versionId);

	/**
	* Retrieve a list of all leaderboards
	*
	* Service Name - leaderboard
	* Service Operation - LIST_ALL_LEADERBOARDS
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *ListAllLeaderboards(UBrainCloudWrapper *brainCloudWrapper);

	/**
	* Gets a player's score from a leaderboard
	*
	* Service Name - leaderboard
	* Service Operation - GET_PLAYER_SCORE
	*
	* Param - leaderboardId The leaderboard ID
	* Param - versionId The version of the leaderboard. Use -1 for current.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetPlayerScore(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, int32 versionId);

	/**
	* Gets a player's beat scores from a leaderboard
	*
	* Service Name - leaderboard
	* Service Operation - GET_PLAYER_SCORES
	*
	* Param - leaderboardId The leaderboard ID
	* Param - versionId The version of the leaderboard. Use -1 for current.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetPlayerScores(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, int32 versionId, int32 maxResults);


	/**
	* Gets a player's score from multiple leaderboards
	*
	* Service Name - leaderboard
	* Service Operation - GET_PLAYER_SCORES_FROM_LEADERBOARDS
	*
	* @Param - leaderboardIds A collection of leaderboardIds to retrieve scores from
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetPlayerScoresFromLeaderboards(UBrainCloudWrapper *brainCloudWrapper, const TArray<FString> leaderboardIds);

		/**
	* Posts score to groups leaderboard - Note the user must be a member of the group
	*
	* Service Name - leaderboard
	* Service Operation - POST_GROUP_SCORE
	*
	* @param leaderboardId A collection of leaderboardIds to retrieve scores from
	* @param groupId the groups Id
	* @param score the score you wish to post
	* @param jsonData extra json Data
	* @param callback The method to be invoked when the server response is received
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToGroupLeaderboard(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId, int32 score, const FString &jsonOtherData);

	/**
	* Post the group's score to the given social leaderboard, dynamically creating the group leaderboard if it does not exist yet.
	* To create new leaderboard, configJson must specify leaderboardType, rotationType, resetAt, and retainedCount, at a minimum, with support to optionally specify an expiry in minutes.
	*
	* Service Name - leaderboard
	* Service Operation - POST_GROUP_SCORE_DYNAMIC_USING_CONFIG
	*
	* @param leaderboardId The leaderboard to post to.
	* @param groupId The id of the group.
	* @param score A score to post
	* @param scoreData Optional user-defined data to post with the score.
	* @param configJson Configuration for the leaderboard if it does not exist yet, specified as JSON object. The supporting configuration fields are listed in the following table of configJson fields.
	* @param callback The method to be invoked when the server response is received
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *PostScoreToDynamicGroupLeaderboardUsingConfig(UBrainCloudWrapper* brainCloudWrapper, const FString& leaderboardId, const FString& groupId, int32 score, const FString& scoreData,
		const FString& configJson);

	/**
	* Removes score from group leaderboard
	*
	* Service Name - leaderboard
	* Service Operation - REMOVE_GROUP_SCORE
	*
	* @param leaderboardId A collection of leaderboardIds to retrieve scores from
	* @param groupId the groups Id
	* @param versionId the score you wish to post
	* @param callback The method to be invoked when the server response is received
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *RemoveGroupScore(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId, int32 score, int32 versionId);

	/**
	* Retrieve a view of the group leaderboardsurrounding the current group.
	*
	* Service Name - leaderboard
	* Service Operation - GET_GROUP_LEADERBOARD_VIEW
	*
	* @param leaderboardId A collection of leaderboardIds to retrieve scores from
	* @param groupId the groups Id
	* @param sortOrder the sort order
	* @param beforeCount count of players before current player to include
	* @param afterCount count of players after current player to include
	* @param callback The method to be invoked when the server response is received
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGroupLeaderboardView(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount);

	/**
	* Retrieve a view of the group leaderboard surrounding the current group by the version
	*
	* Service Name - leaderboard
	* Service Operation - GET_GROUP_LEADERBOARD_VIEW
	*
	* @param leaderboardId A collection of leaderboardIds to retrieve scores from
	* @param groupId the groups Id
	* @param versionId the version
	* @param sortOrder the sort order
	* @param beforeCount count of players before current player to include
	* @param afterCount count of players after current player to include
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy *GetGroupLeaderboardViewByVersion(UBrainCloudWrapper *brainCloudWrapper, const FString &leaderboardId, const FString &groupId, int32 versionId, ESortOrder sortOrder, int32 beforeCount, int32 afterCount);

	/**
	* Post the player's score to the given social leaderboard,
	* dynamically creating the leaderboard if it does not exist yet.
	* To create new leaderboard, configJson must specify
	* leaderboardType, rotationType, resetAt, and retainedCount, at a minimum,
	* with support to optionally specify an expiry in minutes.
	*
	* Service Name - leaderboard
	* Service Operation - POST_SCORE_DYNAMIC_USING_CONFIG
	*
	* @param leaderboardId The leaderboard to post to.
	* @param score A score to post.
	* @param scoreData Optional user-defined data to post with the score.
	* @param configJson
	* Configuration for the leaderboard if it does not exist yet, specified as JSON object.
	* Configuration fields supported are:
	*     leaderboardType': Required. Type of leaderboard. Valid values are:
	*         'LAST_VALUE',
	*         'HIGH_VALUE',
	*         'LOW_VALUE',
	*         'CUMULATIVE',
	*         'ARCADE_HIGH',
	*         'ARCADE_LOW';
	*     'rotationType': Required. Type of rotation. Valid values are:
	*         'NEVER',
	*         'DAILY',
	*         'DAYS',
	*         'WEEKLY',
	*         'MONTHLY',
	*         'YEARLY';
	*     'numDaysToRotate': Required if 'DAYS' rotation type, with valid values between 2 and 14; otherwise, null;
	*     'resetAt': UTC timestamp, in milliseconds, at which to rotate the period. Always null if 'NEVER' rotation type;
	*     'retainedCount': Required. Number of rotations (versions) of the leaderboard to retain;
	*     'expireInMins': Optional. Duration, in minutes, before the leaderboard is to automatically expire.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Leaderboard")
	static UBCLeaderboardProxy* PostScoreToDynamicLeaderboardUsingConfig(UBrainCloudWrapper* brainCloudWrapper, const FString& leaderboardId, int32 score, FString scoreData, FString configJson);

};
