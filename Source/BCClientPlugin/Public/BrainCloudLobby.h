// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "IServerCallback.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Dom/JsonObject.h"
#include "Runtime/Launch/Resources/Version.h"

class BrainCloudClient;
class ServiceOperation;
class FPThreadsCriticalSection;

class BCCLIENTPLUGIN_API BrainCloudLobby : public IServerCallback
{
public:
	static const uint8 MAX_PING_CALLS = 4;
	static const uint8 NUM_PING_CALLS_IN_PARALLEL = 2;
	BrainCloudLobby(BrainCloudClient *client);

	/**
	 * Finds a lobby matching the specified parameters
	 *
	 * Service Name - lobby
	 * Service Operation - FIND_LOBBY
	 *
	 * @param in_roomType type of room
	 * @param in_rating rating of the room
	 * @param in_maxSteps max iterations to search for a lobby
	 * @param in_algoJson json string of the search algorithm to use
	 * @param in_filterJson json string of the filter to be passed on
	 * @param in_timeoutSecs numberOfseconds before timing out
	 * @param in_isReady when lobby is found, place this user as "Ready"
	 * @param in_extraJson json string for extra customization
	 * @param in_teamCode team code
	 * @param in_otherUserCxIds array of other user Connection Ids to bring when the lobby is found
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	[[deprecated("Use the findLobby function that does not contain the in_timeoutSecs parameter")]]
	void findLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
				   const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
				   bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds,
				   IServerCallback *in_callback);

	/**
	 * Finds a lobby matching the specified parameters
	 *
	 * Service Name - lobby
	 * Service Operation - FIND_LOBBY
	 *
	 * @param in_roomType type of room
	 * @param in_rating rating of the room
	 * @param in_maxSteps max iterations to search for a lobby
	 * @param in_algoJson json string of the search algorithm to use
	 * @param in_filterJson json string of the filter to be passed on
	 * @param in_timeoutSecs numberOfseconds before timing out
	 * @param in_isReady when lobby is found, place this user as "Ready"
	 * @param in_extraJson json string for extra customization
	 * @param in_teamCode team code
	 * @param in_otherUserCxIds array of other user Connection Ids to bring when the lobby is found
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void findLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
				   const FString &in_algoJson, const FString &in_filterJson,
				   bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds,
				   IServerCallback *in_callback);

	/**
	 * Begins matchmaking using ping data to select the best region.
	 *
	 * Service Name - Lobby
	 * Service Operation - FindLobbyWithPingData
	 *
	 * @param in_lobbyType The type of lobby to search for
	 * @param in_rating The skill rating used for matchmaking
	 * @param in_maxSteps Maximum number of matchmaking steps
	 * @param in_jsonAlgo Matchmaking algorithm configuration
	 * @param in_jsonFilter Matchmaking filter criteria
	 * @param in_otherUserCxIds Other users to include in the lobby
	 * @param in_isReady Initial ready state of this user
	 * @param in_extraJson Initial extra data for this user
	 * @param in_teamCode Preferred team code, or empty for auto assignment
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void findLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
							   const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
							   bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds,
							   IServerCallback *in_callback);

	/**
	 * Creates a new lobby.
	 *
	 * Service Name - Lobby
	 * Service Operation - CreateLobby
	 *
	 * @param in_lobbyType The type of lobby to create
	 * @param in_rating The skill rating used for matchmaking
	 * @param in_otherUserCxIds Other users to add to the lobby
	 * @param in_isReady Initial ready state of this user
	 * @param in_extraJson Initial extra data for this user
	 * @param in_teamCode Preferred team code, or empty for auto assignment
	 * @param in_jsonSettings Configuration data for the lobby
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void createLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
					 bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
					 const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Creates a new lobby using collected ping data to select the best region.
	 *
	 * Service Name - Lobby
	 * Service Operation - CreateLobbyWithPingData
	 *
	 * @param in_lobbyType The type of lobby to create
	 * @param in_rating The skill rating used for matchmaking
	 * @param in_otherUserCxIds Other users to add to the lobby
	 * @param in_isReady Initial ready state of this user
	 * @param in_extraJson Initial extra data for this user
	 * @param in_teamCode Preferred team code, or empty for auto assignment
	 * @param in_jsonSettings Configuration data for the lobby
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void createLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
								 bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
								 const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Finds or creates a lobby if none are available.
	 *
	 * Service Name - Lobby
	 * Service Operation - FindOrCreateLobby
	 *
	 * @param in_lobbyType The type of lobby
	 * @param in_rating The skill rating used for matchmaking
	 * @param in_maxSteps Maximum number of matchmaking steps
	 * @param in_jsonAlgo Matchmaking algorithm configuration
	 * @param in_jsonFilter Matchmaking filter criteria
	 * @param in_otherUserCxIds Other users to include in the lobby
	 * @param in_jsonSettings Configuration data for the lobby
	 * @param in_isReady Initial ready state of this user
	 * @param in_extraJson Initial extra data for this user
	 * @param in_teamCode Preferred team code, or empty for auto assignment
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void findOrCreateLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
						   const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
						   bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
						   const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Finds or creates a lobby using ping data.
	 *
	 * Service Name - Lobby
	 * Service Operation - FindOrCreateLobbyWithPingData
	 *
	 * @param in_lobbyType The type of lobby
	 * @param in_rating The skill rating used for matchmaking
	 * @param in_maxSteps Maximum number of matchmaking steps
	 * @param in_jsonAlgo Matchmaking algorithm configuration
	 * @param in_jsonFilter Matchmaking filter criteria
	 * @param in_otherUserCxIds Other users to include in the lobby
	 * @param in_jsonSettings Configuration data for the lobby
	 * @param in_isReady Initial ready state of this user
	 * @param in_extraJson Initial extra data for this user
	 * @param in_teamCode Preferred team code, or empty for auto assignment
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void findOrCreateLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
									   const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
									   bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
									   const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Retrieves full lobby data for the specified lobby.
	 *
	 * Service Name - Lobby
	 * Service Operation - GetLobbyData
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getLobbyData(const FString &in_lobbyID, IServerCallback *in_callback);

	/**
	 * Updates the ready state and extra data for the caller.
	 *
	 * Service Name - Lobby
	 * Service Operation - UpdateReady
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_isReady Updated ready state
	 * @param in_extraJson Updated extra data
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateReady(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, IServerCallback *in_callback);

	/**
	 * Updates the lobby settings.
	 *
	 * Service Name - Lobby
	 * Service Operation - UpdateSettings
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_jsonSettings Updated lobby settings
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateSettings(const FString &in_lobbyID, const FString &in_configJson, IServerCallback *in_callback);

	/**
	 * Switches the caller to a different team within the lobby.
	 *
	 * Service Name - Lobby
	 * Service Operation - SwitchTeam
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_toTeamCode Target team code
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void switchTeam(const FString &in_lobbyID, const FString &in_teamCode, IServerCallback *in_callback);

	/**
	 * Sends a signal to all lobby members.
	 *
	 * Service Name - Lobby
	 * Service Operation - SendSignal
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_jsonSignalData Signal payload to send
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendSignal(const FString &in_lobbyID, const FString &in_signalJson, IServerCallback *in_callback);

	/**
	 * Joins the specified lobby.
	 *
	 * Service Name - Lobby
	 * Service Operation - JoinLobby
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_isReady Initial ready state
	 * @param in_extraJson Initial extra data
	 * @param in_teamCode Preferred team code
	 * @param in_otherUserCxIds Other users to include
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void joinLobby(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Joins the specified lobby using ping data.
	 *
	 * Service Name - Lobby
	 * Service Operation - JoinLobbyWithPingData
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_isReady Initial ready state
	 * @param in_extraJson Initial extra data
	 * @param in_teamCode Preferred team code
	 * @param in_otherUserCxIds Other users to include
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void joinLobbyWithPingData(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

	/**
	 * Leaves the specified lobby.
	 *
	 * Service Name - Lobby
	 * Service Operation - LeaveLobby
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void leaveLobby(const FString &in_lobbyID, IServerCallback *in_callback);

	/**
	 * Removes a member from the lobby. Caller must be the lobby owner.
	 *
	 * Service Name - Lobby
	 * Service Operation - RemoveMember
	 *
	 * @param in_lobbyId The lobby identifier
	 * @param in_cxId The cxId of the member to remove
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void removeMember(const FString &in_lobbyID, const FString &in_connectionId, IServerCallback *in_callback);

	/**
	 * Cancels an active find, join, or search request for lobbies.
	 *
	 * @param in_lobbyType The lobby type associated with the request
	 * @param in_entryId The entry identifier returned from matchmaking
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void cancelFindRequest(const FString &in_lobbyType, const FString &in_entryId, IServerCallback *in_callback);

	/**
	 * Retrieves the region settings for each of the given lobby types.
	 * Upon success, pingRegions should be called to collect ping data.
	 *
	 * Service Name - Lobby
	 * Service Operation - GetRegionsForLobbies
	 *
	 * @param in_roomTypes Ids of the lobby types
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getRegionsForLobbies(const TArray<FString> &in_roomTypes, IServerCallback *in_callback);

	/**
	 * Retrieves associated ping data averages to be used with all
	 * Lobby APIs that support ping data.
	 * Must be called after getRegionsForLobbies completes successfully.
	 *
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void pingRegions(IServerCallback *in_callback);

	/**
	 * Retrieves visible lobby instances matching the given criteria.
	 *
	 * Service Name - Lobby
	 * Service Operation - GET_LOBBY_INSTANCES
	 *
	 * @param in_lobbyType The type of lobby
	 * @param in_criteriaJson JSON filter criteria
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getLobbyInstances(const FString &in_lobbyType, const FString &in_criteriaJson, IServerCallback *in_callback);

	/**
	 * Retrieves visible lobby instances matching the given criteria using ping data.
	 *
	 * Service Name - Lobby
	 * Service Operation - GET_LOBBY_INSTANCES_WITH_PING_DATA
	 *
	 * @param in_lobbyType The type of lobby
	 * @param in_criteriaJson JSON filter criteria
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getLobbyInstancesWithPingData(const FString &in_lobbyType, const FString &in_criteriaJson, IServerCallback *in_callback);

	virtual void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, const FString &jsonData);
	virtual void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int32 statusCode, int32 reasonCode, const FString &message);

private:
	void attachPingDataAndSend(TSharedRef<FJsonObject> message, ServiceOperation serviceOperation, IServerCallback *in_callback);
	void pingHost(FString in_region, FString in_target, int in_index);
	void onPingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void pingNextItemToProcess();

	FHttpModule *_http;

	BrainCloudClient *_client = nullptr;
	IServerCallback *_regionsForLobbiesCallback;
	IServerCallback *_pingRegionsCallback;

	TSharedPtr<FJsonObject> _regionPingData;
	TSharedPtr<FJsonObject> _pingData;
	TMap<FString, TArray<double>> _cachedPingResponses;
	TArray<TPair<FString, FString>> m_regionTargetsToProcess;

	FCriticalSection Mutex;
};
