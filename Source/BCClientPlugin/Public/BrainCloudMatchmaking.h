// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudMatchmaking
{
public:
     BrainCloudMatchmaking(BrainCloudClient *client);

     /**
      * Read match making record
      *
      * Service Name - matchMaking
      * Service Operation - READ
      *
      * @param callback The method to be invoked when the server response is received
      */
     void read(IServerCallback *callback);

     /**
      * Sets player rating
      *
      * Service Name - matchMaking
      * Service Operation - SET_PLAYER_RATING
      *
      * @param playerRating The new player rating.
      * @param callback The method to be invoked when the server response is received
      */
     void setPlayerRating(int32 playerRating, IServerCallback *callback);

     /**
      * Resets player rating
      *
      * Service Name - matchMaking
      * Service Operation - RESET_PLAYER_RATING
      *
      * @param callback The method to be invoked when the server response is received
      */
     void resetPlayerRating(IServerCallback *callback);

     /**
      * Increments player rating
      *
      * Service Name - matchMaking
      * Service Operation - INCREMENT_PLAYER_RATING
      *
      * @param increment The increment amount
      * @param callback The method to be invoked when the server response is received
      */
     void incrementPlayerRating(int32 increment, IServerCallback *callback);

     /**
      * Decrements player rating
      *
      * Service Name - matchMaking
      * Service Operation - DECREMENT_PLAYER_RATING
      *
      * @param decrement The decrement amount
      * @param callback The method to be invoked when the server response is received
      */
     void decrementPlayerRating(int32 decrement, IServerCallback *callback);

     /**
      * Turns shield on
      *
      * Service Name - matchMaking
      * Service Operation - SHIELD_ON
      *
      * @param callback The method to be invoked when the server response is received
      */
     void turnShieldOn(IServerCallback *callback);

     /**
      * Turns shield on for the specified number of minutes
      *
      * Service Name - matchMaking
      * Service Operation - SHIELD_ON_FOR
      *
      * @param minutes Number of minutes to turn the shield on for
      * @param callback The method to be invoked when the server response is received
      */
     void turnShieldOnFor(int32 minutes, IServerCallback *callback);

     /**
      * Turns shield off
      *
      * Service Name - matchMaking
      * Service Operation - SHIELD_OFF
      *
      * @param callback The method to be invoked when the server response is received
      */
     void turnShieldOff(IServerCallback *callback);

     /**
      * Increases the shield on time by specified number of minutes
      *
      * Service Name - matchMaking
      * Service Operation - INCREMENT_SHIELD_ON_FOR
      *
      * @param minutes Number of minutes to increase the shield time for
      * @param callback The method to be invoked when the server response is received
      */
     void incrementShieldOnFor(int32 minutes, IServerCallback *callback);

     /**
      * Finds matchmaking enabled players
      *
      * Service Name - matchMaking
      * Service Operation - FIND_PLAYERS
      *
      * @param rangeDelta The range delta
      * @param numMatches The maximum number of matches to return
      * @param callback The method to be invoked when the server response is received
      */
     void findPlayers(int32 rangeDelta, int32 numMatches, IServerCallback *callback);

     /**
      * Finds matchmaking enabled players with additional attributes
      *
      * Service Name - matchMaking
      * Service Operation - FIND_PLAYERS
      *
      * @param rangeDelta The range delta
      * @param numMatches The maximum number of matches to return
      * @param jsonAttributes Attributes match criteria
      * @param callback The method to be invoked when the server response is received
      */
     void findPlayersWithAttributes(int32 rangeDelta, int32 numMatches, const FString &jsonAttributes, IServerCallback *callback);

     /**
      * Finds matchmaking enabled players
      *
      * Service Name - matchMaking
      * Service Operation - FIND_PLAYERS_USING_FILTER
      *
      * @param rangeDelta The range delta
      * @param numMatches The maximum number of matches to return
      * @param jsonExtraParms Parameters to pass to the CloudCode filter script
      * @param callback The method to be invoked when the server response is received
      */
     void findPlayersUsingFilter(int32 rangeDelta, int32 numMatches, const FString &jsonExtraParms, IServerCallback *callback);

     /**
      * Finds matchmaking enabled players using a cloud code filter
      * and additional attributes
      *
      * Service Name - matchMaking
      * Service Operation - FIND_PLAYERS_USING_FILTER
      *
      * @param rangeDelta The range delta
      * @param numMatches The maximum number of matches to return
      * @param jsonAttributes Attributes match criteria
      * @param jsonExtraParms Parameters to pass to the CloudCode filter script
      * @param callback The method to be invoked when the server response is received
      */
     void findPlayersWithAttributesUsingFilter(
         int32 rangeDelta,
         int32 numMatches,
         const FString &jsonAttributes,
         const FString &jsonExtraParms,
         IServerCallback *callback);

     /**
      * Enables Match Making for the Player
      *
      * Service Name - matchMaking
      * Service Operation - ENABLE_MATCH_MAKING
      *
      * @param callback The method to be invoked when the server response is received
      */
     void enableMatchMaking(IServerCallback *callback);

     /**
      * Disables Match Making for the Player
      *
      * Service Name - matchMaking
      * Service Operation - ENABLE_MATCH_MAKING
      *
      * @param callback The method to be invoked when the server response is received
      */
     void disableMatchMaking(IServerCallback *callback);

private:
     BrainCloudClient *_client = nullptr;
};
