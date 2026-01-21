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
     * Finds a lobby matching the specified parameters. Asynchronous - returns 200 to indicate that matchmaking has started. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
     *
     * Service Name - Lobby
     * Service Operation - FindLobbyWithPingData
     *
     * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
     * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
     * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
     * @param algo The algorithm to use for increasing the search scope.
     * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
     * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment
     */
    void findLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
                               const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
                               bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds,
                               IServerCallback *in_callback);

    /**
     * Creates a new lobby.
     *
     * Sends LOBBY_JOIN_SUCCESS message to the user, with full copy of lobby data Sends LOBBY_MEMBER_JOINED to all lobby members, with copy of member data
     *
     * Service Name - Lobby
     * Service Operation - CreateLobby
     *
     * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
     * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
     * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
     * @param settings Configuration data for the room.
     */
    void createLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
                     bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
                     const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
     * Creates a new lobby. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
     *
     * Sends LOBBY_JOIN_SUCCESS message to the user, with full copy of lobby data Sends LOBBY_MEMBER_JOINED to all lobby members, with copy of member data
     *
     * Service Name - Lobby
     * Service Operation - CreateLobbyWithPingData
     *
     * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
     * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
     * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
     * @param settings Configuration data for the room.
     */
    void createLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
                                 bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
                                 const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
     * Adds the caller to the lobby entry queue and will create a lobby if none are found.
     *
     * Service Name - Lobby
     * Service Operation - FindOrCreateLobby
     *
     * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
     * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
     * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
     * @param algo The algorithm to use for increasing the search scope.
     * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
     * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
     * @param settings Configuration data for the room.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
     */
    void findOrCreateLobby(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
                           const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
                           bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
                           const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
     * Adds the caller to the lobby entry queue and will create a lobby if none are found. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
     *
     * Service Name - Lobby
     * Service Operation - FindOrCreateLobbyWithPingData
     *
     * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
     * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
     * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
     * @param algo The algorithm to use for increasing the search scope.
     * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
     * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
     * @param settings Configuration data for the room.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
     */
    void findOrCreateLobbyWithPingData(const FString &in_roomType, int32 in_rating, int32 in_maxSteps,
                                       const FString &in_algoJson, const FString &in_filterJson, int32 in_timeoutSecs,
                                       bool in_isReady, const FString &in_extraJson, const FString &in_teamCode,
                                       const FString &in_configJson, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
     * Returns the data for the specified lobby, including member data.
     *
     * Service Name - Lobby
     * Service Operation - GetLobbyData
     *
     * @param lobbyId Id of chosen lobby.
     */
    void getLobbyData(const FString &in_lobbyID, IServerCallback *in_callback);

    /**
     * Updates the ready status and extra json for the given lobby member.
     *
     * Service Name - Lobby
     * Service Operation - UpdateReady
     *
     * @param lobbyId The type of lobby to look for. Lobby types are defined in the portal.
     * @param isReady Initial ready-status of this user.
     * @param extraJson Initial extra-data about this user.
     */
    void updateReady(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, IServerCallback *in_callback);

    /**
     * Updates the ready status and extra json for the given lobby member.
     *
     * Service Name - Lobby
     * Service Operation - UpdateSettings
     *
     * @param lobbyId Id of the specfified lobby.
     * @param settings Configuration data for the room.
     */
    void updateSettings(const FString &in_lobbyID, const FString &in_configJson, IServerCallback *in_callback);

    /**
     * Switches to the specified team (if allowed.)
     *
     * Sends LOBBY_MEMBER_UPDATED to all lobby members, with copy of member data
     *
     * Service Name - Lobby
     * Service Operation - SwitchTeam
     *
     * @param lobbyId Id of chosen lobby.
     * @param toTeamCode Specified team code.
     */
    void switchTeam(const FString &in_lobbyID, const FString &in_teamCode, IServerCallback *in_callback);

    /**
     * Sends LOBBY_SIGNAL_DATA message to all lobby members.
     *
     * Service Name - Lobby
     * Service Operation - SendSignal
     *
     * @param lobbyId Id of chosen lobby.
     * @param signalData Signal data to be sent.
     */
    void sendSignal(const FString &in_lobbyID, const FString &in_signalJson, IServerCallback *in_callback);

    /**
         * Join specified lobby
         *
         * Service Name - Lobby
         * Service Operation - JoinLobby
         *
         * @param lobbyId Id of the specfified lobby.
         * @param isReady Initial ready-status of this user.
         * @param extraJson Initial extra-data about this user.
         * @param toTeamCode Specified team code.
         * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.

         */
    void joinLobby(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
         * Join specified lobby. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
         *
         * Service Name - Lobby
         * Service Operation - JoinLobbyWithPingData
         *
         * @param lobbyId Id of the specfified lobby.
         * @param isReady Initial ready-status of this user.
         * @param extraJson Initial extra-data about this user.
         * @param toTeamCode Specified team code.
         * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.

         */
    void joinLobbyWithPingData(const FString &in_lobbyID, bool in_isReady, const FString &in_extraJson, const FString &in_teamCode, const TArray<FString> &in_otherUserCxIds, IServerCallback *in_callback);

    /**
     * Causes the caller to leave the specified lobby. If the user was the owner, a new owner will be chosen. If user was the last member, the lobby will be deleted.
     *
     * Service Name - Lobby
     * Service Operation - LeaveLobby
     *
     * @param lobbyId Id of chosen lobby.
     */
    void leaveLobby(const FString &in_lobbyID, IServerCallback *in_callback);

    /**
     * Evicts the specified user from the specified lobby. The caller must be the owner of the lobby.
     *
     * Service Name - Lobby
     * Service Operation - RemoveMember
     *
     * @param lobbyId Id of chosen lobby.
     * @param cxId Specified member to be removed from the lobby.
     */
    void removeMember(const FString &in_lobbyID, const FString &in_connectionId, IServerCallback *in_callback);

    /**
     * Cancel this members Find, Join and Searching of Lobbies
     *
     * Service Name - lobby
     * Service Operation - CANCEL_FIND_REQUEST
     *
     * @param in_lobbyType the lobby type to cancel a find request for
     * @param in_entryId Specific entry id to cancel. Optional.
     * @param in_callback Method to be invoked when the server response is received.
     */
    void cancelFindRequest(const FString &in_lobbyType, const FString &in_entryId, IServerCallback *in_callback);

    /* Retrieves the region settings for each of the given lobby types. Upon success or afterwards, call pingRegions to start retrieving appropriate data.
     *
     * Service Name - Lobby
     * Service Operation - GetRegionsForLobbies
     *
     * @param roomTypes Ids of the lobby types.
     */
    void getRegionsForLobbies(const TArray<FString> &in_roomTypes, IServerCallback *in_callback);

    /* Retrieves associated Ping Data averages to be used with all associated <>WithPingData APIs.
     * Call anytime after GetRegionsForLobbies before proceeding.
     * Once that completes, the associated region Ping Data is retrievable via getPingData and all associated <>WithPingData APIs are useable
     */
    void pingRegions(IServerCallback *in_callback);

    /**
     * Gets a map keyed by rating of the visible lobby instances matching the given type and rating range.
     *
     * Service Name - Lobby
     * Service Operation - GET_LOBBY_INSTANCES
     *
     * @param lobbyType The type of lobby to look for.
     * @param criteriaJson A JSON string used to describe filter criteria.
     */
    void getLobbyInstances(const FString &in_lobbyType, const FString &in_criteriaJson, IServerCallback *in_callback);

    /**
     * Gets a map keyed by rating of the visible lobby instances matching the given type and rating range.
     * Only lobby instances in the regions that satisfy the ping portion of the criteriaJson (based on the values provided in pingData) will be returned.
     *
     * Service Name - Lobby
     * Service Operation - GET_LOBBY_INSTANCES_WITH_PING_DATA
     *
     * @param lobbyType The type of lobby to look for.
     * @param criteriaJson A JSON string used to describe filter criteria.
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
