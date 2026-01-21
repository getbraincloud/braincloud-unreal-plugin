// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudPlayerState
{
     friend class BrainCloudComms;
     friend class BrainCloudComms2;

public:
     BrainCloudPlayerState(BrainCloudClient *client);

     /**
      * Read the state of the currently logged in user.
      * This method returns a JSON object describing most of the
      * user's data: entities, statistics, level, currency.
      * Apps will typically call this method after authenticating to get an
      * up-to-date view of the user's data.
      *
      * Service Name - PlayerState
      * Service Operation - Read
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void readUserState(IServerCallback *callback, const FString &entityTypeFilter = TEXT(""));

     /**
      * Completely deletes the user record and all data fully owned
      * by the user. After calling this method, the user will need
      * to re-authenticate and create a new profile.
      * This is mostly used for debugging/qa.
      *
      * Service Name - PlayerState
      * Service Operation - FullReset
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void deleteUser(IServerCallback *callback);

     /**
      * This method will delete *most* data for the currently logged in user.
      * Data which is not deleted includes: currency, credentials, and
      * purchase transactions. ResetUser is different from DeleteUser in that
      * the user record will continue to exist after the reset (so the user
      * does not need to re-authenticate).
      *
      * Service Name - PlayerState
      * Service Operation - DataReset
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void resetUserState(IServerCallback *callback);

     /**
      * Logs user out of server.
      *
      * Service Name - PlayerState
      * Service Operation - Logout
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void logout(IServerCallback *callback);

     /**
      * Sets the user's name.
      *
      * Service Name - playerState
      * Service Operation - UPDATE_NAME
      *
      * @param in_userName The name of the user
      * @param in_callback The method to be invoked when the server response is received
      */
     void updateUserName(const FString &name, IServerCallback *callback);

     /**
      * Updates the "friend summary data" associated with the logged in user.
      * Some operations will return this summary data. For instance the social
      * leaderboards will return the player's score in the leaderboard along
      * with the friend summary data. Generally this data is used to provide
      * a quick overview of the player without requiring a separate API call
      * to read their public stats or entity data.
      *
      * Service Name - PlayerState
      * Service Operation - UpdateSummary
      *
      * @param in_jsonSummaryData A JSON string defining the summary data.
      * For example:
      * {
      *   "xp":123,
      *   "level":12,
      *   "highScore":45123
      * }
      * @param in_callback Method to be invoked when the server response is received.
      */
     void updateSummaryFriendData(const FString &jsonSummaryData, IServerCallback *callback);

     /**
      * Retrieve the user's attributes.
      *
      * Service Name - PlayerState
      * Service Operation - GetAttributes
      *
      * @param in_callback The method to be invoked when the server response is received
      */
     void getAttributes(IServerCallback *callback);

     /**
      * Update user's attributes.
      *
      * Service Name - PlayerState
      * Service Operation - UpdateAttributes
      *
      * @param in_jsonAttributes Single layer json string that is a set of key-value pairs
      * @param in_wipeExisting Whether to wipe existing attributes prior to update.
      * @param in_callback The method to be invoked when the server response is received
      */
     void updateAttributes(const FString &jsonAttributes, bool wipeExisting, IServerCallback *callback);

     /**
        * Remove user's attributes.
        *
        * Service Name - PlayerState
        * Service Operation - RemoveAttributes
        *
        * @param in_attributeNames Collection of attribute names.
        * @param in_callback The method to be invoked when the server response is received
        */
     void removeAttributes(const TArray<FString> &attributeNames, IServerCallback *callback);

     /**
      * Update User picture URL.
      *
      * Service Name - PlayerState
      * Service Operation - UPDATE_PICTURE_URL
      *
      * @param in_pictureUrl URL to apply
      * @param in_callback The method to be invoked when the server response is received
      */
     void updateUserPictureUrl(const FString &pictureUrl, IServerCallback *callback);

     /**
      * Update the user's contact email.
      * Note this is unrelated to email authentication.
      *
      * Service Name - PlayerState
      * Service Operation - UPDATE_CONTACT_EMAIL
      *
      * @param in_contactEmail Updated email
      * @param in_callback The method to be invoked when the server response is received
      */
     void updateContactEmail(const FString &contactEmail, IServerCallback *callback);

     /**
      * Delete's the specified status
      *
      * Service Name - PlayerState
      * Service Operation - CLEAR_USER_STATUS
      *
      * @param in_statusName Updated email
      * @param in_callback The method to be invoked when the server response is received
      */
     void clearUserStatus(const FString &statusName, IServerCallback *callback);

     /**
         * Stack user's statuses
         *
         * Service Name - PlayerState
         * Service Operation - EXTEND_USER_STATUS
         *
         * @param in_statusName Updated email
         * @param additionalSecs
         * @param details
         * @param in_callback The method to be invoked when the server response is received
         */
     void extendUserStatus(const FString &statusName, int additionalSecs, const FString &details, IServerCallback *callback);

     /**
      * Get user status
      *
      * Service Name - PlayerState
      * Service Operation - GET_USER_STATUS
      *
      * @param in_statusName Updated email
      * @param in_callback The method to be invoked when the server response is received
      */
     void getUserStatus(const FString &statusName, IServerCallback *callback);

     /**
         * Set timed status for a user
         *
         * Service Name - PlayerState
         * Service Operation - SET_USER_STATUS
         *
         * @param in_statusName Updated email
         * @param in_durationSecs
         * @param in_details
         * @param in_callback The method to be invoked when the server response is received
         */
     void setUserStatus(const FString &statusName, int durationSecs, const FString &details, IServerCallback *callback);

     /**
        * Remove user's attributes.
        *
        * Service Name - PlayerState
        * Service Operation - RemoveAttributes
        *
        * @param in_attributeNames Collection of attribute names.
        * @param in_callback The method to be invoked when the server response is received
        */
     void updateTimeZoneOffset(int32 timeZoneOffset, IServerCallback *callback);

     /**
      * Remove user's attributes.
      *
      * Service Name - PlayerState
      * Service Operation - RemoveAttribute
      *
      * @param in_attributeNames Collection of attribute names.
      * @param in_callback The method to be invoked when the server response is received
      */
     void updateLanguageCode(const FString &languageCode, IServerCallback *callback);

     /**
      * Gets a cached value of the currently authenticated user's name
      * @returns The cached currently authenticated user's name
      */
     const FString &getUserName();

protected:
     void setUserName(const FString &name);

private:
     BrainCloudClient *_client = nullptr;
     FString _userName = "";
};
