// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

// defined in Leaderboard, beware name conflict in Experimental Plugin Virtual Camera
enum class EBCSortOrder : uint8;

class BCCLIENTPLUGIN_API BrainCloudTournament
{
public:
	BrainCloudTournament(BrainCloudClient *client);

	/**
	 * Processes any outstanding rewards for the given player
	 *
	 * Service Name - tournament
	 * Service Operation - CLAIM_TOURNAMENT_REWARD
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param versionId Version of the tournament. Use -1 for the latest version.
	 * @param callback The method to be invoked when the server response is received
	 */
	void claimTournamentReward(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

	/**
	 * Get the status of a division
	 *
	 * Service Name - tournament
	 * Service Operation - GET_DIVISION_INFO
	 *
	 * @param divSetId The id for the division
	 * @param callback The method to be invoked when the server response is received
	 */
	void getDivisionInfo(const FString &divSetId, IServerCallback *callback);

	/**
	 * Get the status of a group division
	 *
	 * Service Name - tournament
	 * Service Operation - GET_GROUP_DIVISION_INFO
	 *
	 * @param divSetId The id for the division
	 * @param groupId The group id
	 * @param callback The method to be invoked when the server response is received
	 */
	void getGroupDivisionInfo(const FString& divSetId, const FString& groupId, IServerCallback* callback);

	/**
	 * Returns list of player's recently active divisions
	 *
	 * Service Name - tournament
	 * Service Operation - GET_MY_DIVISIONS
	 *
	 * @param callback The method to be invoked when the server response is received
	 */
	void getMyDivisions(IServerCallback *callback);

	/**
	 * Returns list of groups recently active divisions
	 *
	 * Service Name - tournament
	 * Service Operation - GET_GROUP_DIVISIONS
	 *
	 * @param groupId The group id
	 * @param callback The method to be invoked when the server response is received
	 */
	void getGroupDivisions(const FString& groupId, IServerCallback* callback);

	/**
	 * Get tournament status associated with a leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - GET_TOURNAMENT_STATUS
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param versionId Version of the tournament. Use -1 for the latest version.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getTournamentStatus(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

	/**
	 * Get group tournament status associated with a leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - GET_GROUP_TOURNAMENT_STATUS
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param groupId The group id
	 * @param versionId Version of the tournament. Use -1 for the latest version.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getGroupTournamentStatus(const FString& leaderboardId, const FString& groupId, int32 versionId, IServerCallback* callback = nullptr);

	/**
	 * Join the specified division.
	 * If joining requires a fee, it is possible to fail at joining the division
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_DIVISION
	 *
	 * @param divSetId The id for the division
	 * @param tournamentCode Tournament to join
	 * @param initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void joinDivision(const FString &divSetId, const FString &tournamentCode, int32 initialScore, IServerCallback *callback);

	/**
	 * Join the specified group division.
	 * If joining requires a fee, it is possible to fail at joining the division
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_GROUP_DIVISION
	 *
	 * @param divSetId The id for the division
	 * @param tournamentCode Tournament to join
	 * @param groupId The group id
	 * @param initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void joinGroupDivision(const FString& divSetId, const FString& tournamentCode, const FString& groupId, int32 initialScore, IServerCallback* callback);

	/**
	 * Join the specified tournament.
	 * Any entry fees will be automatically collected.
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_TOURNAMENT
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param tournamentCode Tournament to join
	 * @param initialScore The initial score for players first joining a tournament
	 *						  Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void joinTournament(const FString &leaderboardId, const FString &tournamentCode, int32 initialScore, IServerCallback *callback = nullptr);

	/**
	 * Join the specified group tournament.
	 * Any entry fees will be automatically collected.
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_GROUP_TOURNAMENT
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param tournamentCode Tournament to join
	 * @param groupId The group id
	 * @param initialScore The initial score for players first joining a tournament
	 *						  Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void joinGroupTournament(const FString& leaderboardId, const FString& tournamentCode, const FString& groupId, int32 initialScore, IServerCallback* callback = nullptr);

	/**
	 * Removes player from division instance
	 * Also removes division instance from player's division list
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_DIVISION_INSTANCE
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param callback The method to be invoked when the server response is received
	 */
	void leaveDivisionInstance(const FString& leaderboardId, IServerCallback *callback);

	/**
	 * Removes player from group division instance
	 * Also removes group division instance from player's division list
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_GROUP_DIVISION_INSTANCE
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param groupId The group id
	 * @param callback The method to be invoked when the server response is received
	 */
	void leaveGroupDivisionInstance(const FString& leaderboardId, const FString& groupId, IServerCallback* callback);

	/**
	 * Removes player's score from tournament leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_TOURNAMENT
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param callback The method to be invoked when the server response is received
	 */
	void leaveTournament(const FString& leaderboardId, IServerCallback *callback = nullptr);

	/**
	 * Removes player's score from group tournament leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_GROUP_TOURNAMENT
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param groupId The group id
	 * @param callback The method to be invoked when the server response is received
	 */
	void leaveGroupTournament(const FString& leaderboardId, const FString& groupId, IServerCallback* callback = nullptr);

	/**
	 * Post the users score to the leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_TOURNAMENT_SCORE
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param score The score to post
	 * @param jsonData Optional data attached to the leaderboard entry
	 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param callback The method to be invoked when the server response is received
	 */
	void postTournamentScoreUTC(const FString &leaderboardId, int32 score, const FString &jsonData, int64 roundStartTimeUTC, IServerCallback *callback = nullptr);

	/**
	 * Post the users score to the group leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_GROUP_TOURNAMENT_SCORE
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param groupId The group id
	 * @param score The score to post
	 * @param jsonData Optional data attached to the leaderboard entry
	 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param callback The method to be invoked when the server response is received
	 */
	void postGroupTournamentScore(const FString& leaderboardId, const FString& groupId, int32 score, const FString& jsonData, int64 roundStartTimeUTC, IServerCallback* callback = nullptr);

	/**
	 * Post the users score to the leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_TOURNAMENT_SCORE_WITH_RESULTS
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param score The score to post
	 * @param jsonData Optional data attached to the leaderboard entry
	 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param sort Sort key Sort order of page.
	 * @param beforeCount The count of number of players before the current player to include.
	 * @param afterCount The count of number of players after the current player to include.
	 * @param initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void postTournamentScoreWithResultsUTC(const FString &leaderboardId, int32 score, const FString &jsonData, int64 roundStartTimeUTC, EBCSortOrder sort, int32 beforeCount, int32 afterCount, float initialScore, IServerCallback *callback);

	/**
	 * Post the users score to the group leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_GROUP_TOURNAMENT_SCORE_WITH_RESULTS
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param groupId The group id
	 * @param score The score to post
	 * @param jsonData Optional data attached to the leaderboard entry
	 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param sort Sort key Sort order of page.
	 * @param beforeCount The count of number of players before the current player to include.
	 * @param afterCount The count of number of players after the current player to include.
	 * @param initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param callback The method to be invoked when the server response is received
	 */
	void postGroupTournamentScoreWithResults(const FString& leaderboardId, const FString& groupId,int32 score, const FString& jsonData, int64 roundStartTimeUTC, EBCSortOrder sort, int32 beforeCount, int32 afterCount, float initialScore, IServerCallback* callback);

	/**
	 * Returns the user's expected reward based on the current scores
	 *
	 * Service Name - tournament
	 * Service Operation - VIEW_CURRENT_REWARD
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param callback The method to be invoked when the server response is received
	 */
	void viewCurrentReward(const FString &leaderboardId, IServerCallback *callback = nullptr);

	/**
	 * Returns the user's reward from a finished tournament
	 *
	 * Service Name - tournament
	 * Service Operation - VIEW_REWARD
	 *
	 * @param leaderboardId The leaderboard for the tournament
	 * @param versionId Version of the tournament. Use -1 for the latest version.
	 * @param callback The method to be invoked when the server response is received
	 */
	void viewReward(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
	FString tournamentSortOrderToString(EBCSortOrder type);
};
