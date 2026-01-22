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
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_versionId Version of the tournament. Use -1 for the latest version.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void claimTournamentReward(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

	/**
	 * Get the status of a division
	 *
	 * Service Name - tournament
	 * Service Operation - GET_DIVISION_INFO
	 *
	 * @param in_divSetId The id for the division
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getDivisionInfo(const FString &divSetId, IServerCallback *callback);

	/**
	 * Returns list of player's recently active divisions
	 *
	 * Service Name - tournament
	 * Service Operation - GET_MY_DIVISIONS
	 *
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getMyDivisions(IServerCallback *callback);

	/**
	 * Get tournament status associated with a leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - GET_TOURNAMENT_STATUS
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_versionId Version of the tournament. Use -1 for the latest version.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getTournamentStatus(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

	/**
	 * Join the specified division.
	 * If joining requires a fee, it is possible to fail at joining the division
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_DIVISION
	 *
	 * @param in_divSetId The id for the division
	 * @param in_tournamentCode Tournament to join
	 * @param in_initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void joinDivision(const FString &divSetId, const FString &tournamentCode, int32 initialScore, IServerCallback *callback);

	/**
	 * Join the specified tournament.
	 * Any entry fees will be automatically collected.
	 *
	 * Service Name - tournament
	 * Service Operation - JOIN_TOURNAMENT
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_tournamentCode Tournament to join
	 * @param in_initialScore The initial score for players first joining a tournament
	 *						  Usually 0, unless leaderboard is LOW_VALUE
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void joinTournament(const FString &leaderboardId, const FString &tournamentCode, int32 initialScore, IServerCallback *callback = nullptr);

	/**
	 * Removes player from division instance
	 * Also removes division instance from player's division list
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_DIVISION_INSTANCE
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void leaveDivisionInstance(const FString &leaderboardId, IServerCallback *callback);

	/**
	 * Removes player's score from tournament leaderboard
	 *
	 * Service Name - tournament
	 * Service Operation - LEAVE_TOURNAMENT
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void leaveTournament(const FString &leaderboardId, IServerCallback *callback = nullptr);

	/**
	 * Post the users score to the leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_TOURNAMENT_SCORE
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_score The score to post
	 * @param in_jsonData Optional data attached to the leaderboard entry
	 * @param in_roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void postTournamentScoreUTC(const FString &leaderboardId, int32 score, const FString &jsonData, int64 roundStartTimeUTC, IServerCallback *callback = nullptr);

	/**
	 * Post the users score to the leaderboard - UTC time
	 *
	 * Service Name - tournament
	 * Service Operation - POST_TOURNAMENT_SCORE_WITH_RESULTS
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_score The score to post
	 * @param in_jsonData Optional data attached to the leaderboard entry
	 * @param in_roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
	 * @param in_sort Sort key Sort order of page.
	 * @param in_beforeCount The count of number of players before the current player to include.
	 * @param in_afterCount The count of number of players after the current player to include.
	 * @param in_initialScore The initial score for players first joining a tournament
	 *						 Usually 0, unless leaderboard is LOW_VALUE
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void postTournamentScoreWithResultsUTC(const FString &leaderboardId, int32 score, const FString &jsonData, int64 roundStartTimeUTC, EBCSortOrder sort, int32 beforeCount, int32 afterCount, float initialScore, IServerCallback *callback);

	/**
	 * Returns the user's expected reward based on the current scores
	 *
	 * Service Name - tournament
	 * Service Operation - VIEW_CURRENT_REWARD
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void viewCurrentReward(const FString &leaderboardId, IServerCallback *callback = nullptr);

	/**
	 * Returns the user's reward from a finished tournament
	 *
	 * Service Name - tournament
	 * Service Operation - VIEW_REWARD
	 *
	 * @param in_leaderboardId The leaderboard for the tournament
	 * @param in_versionId Version of the tournament. Use -1 for the latest version.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void viewReward(const FString &leaderboardId, int32 versionId, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
	FString tournamentSortOrderToString(EBCSortOrder type);
};
