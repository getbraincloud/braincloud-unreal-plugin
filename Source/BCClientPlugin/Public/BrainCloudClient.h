// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once
#include "BrainCloudACL.h"
#include "BrainCloudAppDataStruct.h"
#include "BrainCloudAppStore.h"
#include "BrainCloudAsyncMatch.h"
#include "BrainCloudAuthentication.h"
#include "BrainCloudBlockchain.h"
#include "BrainCloudChat.h"
#include "BrainCloudCustomEntity.h"
#include "BrainCloudDataStream.h"
#include "BrainCloudEntity.h"
#include "BrainCloudEvent.h"
#include "BrainCloudFile.h"
#include "BrainCloudFriend.h"
#include "BrainCloudFunctionLibrary.h"
#include "BrainCloudGamification.h"
#include "BrainCloudGlobalApp.h"
#include "BrainCloudGlobalEntity.h"
#include "BrainCloudGlobalFile.h"
#include "BrainCloudGlobalStatistics.h"
#include "BrainCloudGroup.h"
#include "BrainCloudGroupACL.h"
#include "BrainCloudGroupFile.h"
#include "BrainCloudIdentity.h"
#include "BrainCloudItemCatalog.h"
#include "BrainCloudLeaderboard.h"
#include "BrainCloudLobby.h"
#include "BrainCloudMail.h"
#include "BrainCloudMatchmaking.h"
#include "BrainCloudMessaging.h"
#include "BrainCloudOneWayMatch.h"
#include "BrainCloudPlaybackStream.h"
#include "BrainCloudPlayerState.h"
#include "BrainCloudPlayerStatistics.h"
#include "BrainCloudPlayerStatisticsEvent.h"
#include "BrainCloudPresence.h"
#include "BrainCloudProfanity.h"
#include "BrainCloudPushNotification.h"
#include "BrainCloudRTT.h"
#include "BrainCloudRedemptionCode.h"
#include "BrainCloudRelay.h"
#include "BrainCloudS3Handling.h"
#include "BrainCloudScript.h"
#include "BrainCloudTime.h"
#include "BrainCloudTimeUtils.h"
#include "BrainCloudTournament.h"
#include "BrainCloudUserItems.h"
#include "BrainCloudVirtualCurrency.h"
#include "Kismet/GameplayStatics.h"
#include "BrainCloudClient.generated.h"

class BrainCloudComms;
class BrainCloudRTTComms;
class BrainCloudRelayComms;
class ServerCall;
class IEventCallback;
class IRewardCallback;
class IFileUploadCallback;
class IGlobalErrorCallback;
class INetworkErrorCallback;
class IRTTCallback;
class UBCBlueprintRTTCallProxyBase;
class UBCBlueprintRestCallProxyBase;

UENUM(BlueprintType)
enum class eBCUpdateType : uint8
{
	ALL UMETA(DisplayName = "ALL"),
	REST UMETA(DisplayName = "REST"),
	RTT UMETA(DisplayName = "RTT"),
	RS UMETA(DisplayName = "RS")
};

UENUM(BlueprintType)
enum class BCRTTConnectionType : uint8
{
	WEBSOCKET UMETA(DisplayName = "WEBSOCKET")
};

UENUM(BlueprintType)
enum class BCRelayConnectionType : uint8
{
	WEBSOCKET UMETA(DisplayName = "WEBSOCKET"),
	TCP UMETA(DisplayName = "TCP"),
	UDP UMETA(DisplayName = "UDP")
};

UENUM(BlueprintType)
enum class BCRelayChannel : uint8
{
	HighPriority1 UMETA(DisplayName = "HighPriority1"),
	HighPriority2 UMETA(DisplayName = "HighPriority2"),
	NormalPriority UMETA(DisplayName = "NormalPriority"),
	LowPriority UMETA(DisplayName = "LowPriority")
};

UENUM(BlueprintType)
enum class BCRTTConnectionStatus: uint8
{
	CONNECTED UMETA(DisplayName = "CONNECTED"),
	CONNECTING UMETA(DisplayName = "CONNECTING"),
	DISCONNECTED UMETA(DisplayName = "DISCONNECTED"),
	DISCONNECTING UMETA(DisplayName = "DISCONNECTING")
};

UENUM(BlueprintType)
enum class BCWebsocketStatus: uint8
{
	OPEN UMETA(DisplayName = "OPEN"),
	CLOSED UMETA(DisplayName = "CLOSED"),
	MESSAGE UMETA(DisplayName = "MESSAGE"),
	SOCKETERROR UMETA(DisplayName = "SOCKETERROR"),
	NONE UMETA(DisplayName = "NONE")

};

class BCCLIENTPLUGIN_API BrainCloudClient
{
  public:
	/** Public constants */
	static bool SINGLE_THREADED;

	BrainCloudClient();
	//  void BeginDestroy() override;
	~BrainCloudClient();

	/**
	* Enables / Disables compression of both API requests and responses
	*
	* @param isEnabled Boolean to decide whether to enable or disable compression
	*/
	void EnableCompression(bool isEnabled);

	/**
	* Enables / Disables compression of API requests, enabled by default
	*
	* @param isEnabled Boolean to decide whether to enable or disable compression
	*/
	void EnableCompressedRequests(bool isEnabled);

	/**
	* Enables / Disables compression of API responses, enabled by default
	*
	* @param isEnabled Boolean to decide whether to enable or disable compression
	*/
	void EnableCompressedResponses(bool isEnabled);

	/**
	* Method initializes the BrainCloudClient.
	*
	* @param serverURL The url to the brainCloud server
	* @param secretKey The secret key for your app
	* @param appId The app's id
	* @param appVersion The app's version
	*/
	void initialize(const FString &serverUrl,
					const FString &secretKey,
					const FString &appId,
					const FString &appVersion);
	/**
	* Method initializes the BrainCloudClient with multiple app/secret.
	* Used when needed to switch between child and parent apps
	*
	* @param serverURL The url to the brainCloud server
	* @param appId The app's id
	* @param appIdSecretMap is the map of <appId, secretKey>
	* @param appVersion The app's version
	*/
	void initializeWithApps(const FString &serverUrl,
							const FString &appId,
							const TMap<FString, FString> &secretMap,
							const FString &appVersion);

	/**
	* Initialize - initializes the identity service with the saved
	* anonymous installation id and most recently used profile id
	*
	* @param profileId The id of the profile id that was most recently used by the app (on this device)
	* @param anonymousId  The anonymous installation id that was generated for this device
	*/
	void initializeIdentity(const FString &profileId, const FString &anonymousId);

	/**
	* Initializes device info such as country code, language and time zone
	*/
	void InitDeviceInfo();

	/**
	* Run callbacks, to be called once per frame from your main thread
	*/
	void runCallbacks(eBCUpdateType in_updateType = eBCUpdateType::ALL);

	/**
	* Sets a callback handler for any out of band event messages that come from
	* brainCloud.
	*
	* @param eventCallback A function which takes a json string as it's only parameter.
	* The json format looks like the following:
	* {
	*    "events": [{
	*       "fromPlayerId": "178ed06a-d575-4591-8970-e23a5d35f9df",
	*       "eventId": 3967,
	*       "createdAt": 1441742105908,
	*       "gameId": "123",
	*       "toPlayerId": "178ed06a-d575-4591-8970-e23a5d35f9df",
	*       "eventType": "test",
	*       "eventData": {"testData": 117}
	*     }],
	*     ]
	*   }
	*/
	void registerEventCallback(IEventCallback *eventCallback);
	void registerEventCallback(UBCBlueprintRestCallProxyBase *eventCallback); // blueprint support

	/**
	* Deregisters the event callback
	*/
	void deregisterEventCallback();

	/**
	* Sets a reward handler for any api call results that return rewards.
	*
	* @param rewardCallback The reward callback handler.
	* @see The brainCloud apidocs site for more information on the return JSON
	*/
	void registerRewardCallback(IRewardCallback *rewardCallback);
	void registerRewardCallback(UBCBlueprintRestCallProxyBase *rewardCallback); // blueprint support

	/**
	* Deregisters the reward callback
	*/
	void deregisterRewardCallback();

	/**
	* Registers a file upload callback handler to listen for status updates on uploads
	*
	* @param fileUploadCallback The file upload callback handler.
	*/
	void registerFileUploadCallback(IFileUploadCallback *fileUploadCallback);
	void registerFileUploadCallback(UBCBlueprintRestCallProxyBase *fileUploadCallback); // blueprint support

	/**
	* Deregisters the file upload callback
	*/
	void deregisterFileUploadCallback();

	/**
	* Registers a callback that is invloked for all errors generated
	*
	* @param globalErrorCallback The global error callback handler.
	*/
	void registerGlobalErrorCallback(IGlobalErrorCallback *globalErrorCallback);
	void registerGlobalErrorCallback(UBCBlueprintRestCallProxyBase *globalErrorCallback); // blueprint support

	/**
	* Registers a callback that is invoked for network errors.
	* Note this is only called if enableNetworkErrorMessageCaching
	* has been set to true.
	*
	* @param networkErrorCallback The network error callback handler.
	*/
	void registerNetworkErrorCallback(INetworkErrorCallback *networkErrorCallback);
	void registerNetworkErrorCallback(UBCBlueprintRestCallProxyBase *networkErrorCallback); // blueprint support

	/**
	* Deregisters the network error callback
	*/
	void deregisterNetworkErrorCallback();

	/**
	* Deregisters the global error callback
	*/
	void deregisterGlobalErrorCallback();

	/**
	* Set to true to enable logging packets to the output log
	*/
	void enableLogging(bool shouldEnable);

	/**
	* 
	*/
	bool isLoggingEnabled();

	/**
	* Returns whether the client is authenticated with the brainCloud server.
	* @return True if authenticated, false otherwise.
	*/
	bool isAuthenticated();

	/**
	* Returns whether the client is initialized.
	* @return True if initialized, false otherwise.
	*/
	bool isInitialized();

	/**
	* Send an empty message to the server, which essentially polls the
	* server for any new events to send to this client.
	*/
	void heartbeat();

	/**
	* Sends a service request message to the server. This will most likely be placed
	* in a queue...
	*
	* @param serviceMessage
	*/
	void sendRequest(ServerCall *serviceMessage);

	/**
	* Clears any pending messages from communication library.
	*/
	void resetCommunication();

	//Getters
	BrainCloudAuthentication *getAuthenticationService();
	BrainCloudLeaderboard *getLeaderboardService();
	BrainCloudPlayerState *getPlayerStateService();
	BrainCloudGamification *getGamificationService();
	BrainCloudGlobalEntity *getGlobalEntityService();
	BrainCloudGlobalStatistics *getGlobalStatisticsService();
	BrainCloudEntity *getEntityService();
	BrainCloudPlayerStatistics *getPlayerStatisticsService();
	BrainCloudTime *getTimeService();
	BrainCloudPlayerStatisticsEvent *getPlayerStatisticsEventService();
	BrainCloudIdentity *getIdentityService();
	BrainCloudItemCatalog *getItemCatalogService();
	BrainCloudUserItems *getUserItemsService();
	BrainCloudEvent *getEventService();
	BrainCloudS3Handling *getS3HandlingService();
	BrainCloudScript *getScriptService();
	BrainCloudAsyncMatch *getAsyncMatchService();
	BrainCloudFriend *getFriendService();
	BrainCloudGlobalApp *getGlobalAppService();
	BrainCloudMatchmaking *getMatchmakingService();
	BrainCloudOneWayMatch *getOneWayMatchService();
	BrainCloudPlaybackStream *getPlaybackStreamService();
	BrainCloudPushNotification *getPushNotificationService();
	BrainCloudRedemptionCode *getRedemptionCodeService();
	BrainCloudDataStream *getDataStreamService();
	BrainCloudProfanity *getProfanityService();
	BrainCloudFile *getFileService();
	BrainCloudGroup *getGroupService();
	BrainCloudMail *getMailService();
	BrainCloudTournament *getTournamentService();
	BrainCloudGlobalFile *getGlobalFileService();
	BrainCloudCustomEntity *getCustomEntityService();
	BrainCloudPresence *getPresenceService();
	BrainCloudVirtualCurrency *getVirtualCurrencyService();
	BrainCloudAppStore *getAppStoreService();
	BrainCloudRTT *getRTTService();
	BrainCloudLobby *getLobbyService();
	BrainCloudChat *getChatService();
	BrainCloudMessaging *getMessagingService();
	BrainCloudRelay *getRelayService();
	BrainCloudTimeUtils *getUtil();
	BrainCloudBlockchain *getBlockchainService();
	BrainCloudGroupFile *getGroupFileService();
	
	const FString &getAppId() { return _appId; };
	const FString &getSessionId();
	const FString &getReleasePlatform() { return _releasePlatform; };
	const FString &getProfileId() { return getAuthenticationService()->getProfileId(); };

	const FString &getEventServer();

	const FString &getAppVersion() { return _appVersion; };
	const FString &getBrainCloudClientVersion() { return s_brainCloudClientVersion; };

	const FString &getCountryCode() { return _country; }
	const FString &getLanguageCode() { return _language; }
	const float getTimezoneOffset() { return _timezoneOffset; }

	const TArray<int32> &getPacketTimeouts();

	BrainCloudComms *getBrainCloudComms() { return _brainCloudComms; }

	/**
	* Gets the authentication packet timeout which is tracked separately
	* from all other packets. Note that authentication packets are never
	* retried and so this value represents the total time a client would
	* wait to receive a reply to an authentication api call. By default
	* this timeout is set to 15 seconds.
	*
	* @return The timeout in seconds
	*/
	int32 getAuthenticationPacketTimeout();

	/**
	* Sets the authentication packet timeout which is tracked separately
	* from all other packets. Note that authentication packets are never
	* retried and so this value represents the total time a client would
	* wait to receive a reply to an authentication api call. By default
	* this timeout is set to 15 seconds.
	*
	* @param timeoutSecs The timeout in seconds
	*/
	void setAuthenticationPacketTimeout(int32 timeoutSecs);

	/**
	 * set an interval in ms for which the BrainCloud will contact the server
	 * and receive any pending events
	 *
	 * @param intervalInMilliseconds The time between heartbeats in milliseconds
	 */
	void setHeartbeatInterval(int32 intervalInMilliseconds);

	/**
	* Sets the packet timeouts using a list of integers that
	* represent timeout values in seconds for each packet retry. The
	* first item in the list represents the timeout for the first packet
	* attempt, the second for the second packet attempt, and so on.
	*
	* The number of entries in this array determines how many packet
	* retries will occur.
	*
	* By default, the packet timeout array is {10, 10, 10}
	*
	* @param timeouts An array of packet timeouts.
	*/
	void setPacketTimeouts(const TArray<int32> &timeouts);

	/**
	* Sets the packet timeouts back to the default ie {10, 10, 10}
	*/
	void setPacketTimeoutsToDefault();

	/**
	* Sets the error callback to return the status message instead of the
	* error json string. This flag is used to conform to pre-2.17 client
	* behaviour.
	*
	* @param enabled If set to true, enable
	*/
	void setOldStyleStatusMessageErrorCallback(bool enabled);

	/**
	* Sets whether the error callback is triggered when a 202 status
	* is received from the server. By default this is true and should
	* only be set to false for backward compatibility.
	*
	* @param isError If set to true, 202 is treated as an error
	*/
	void setErrorCallbackOn202Status(bool isError);

	/**
	* Returns the low transfer rate timeout in secs
	*
	* @returns The low transfer rate timeout in secs
	*/
	int32 getUploadLowTransferRateTimeout();

	/**
	* Sets the timeout in seconds of a low speed upload
	* (ie transfer rate which is underneath the low transfer rate threshold).
	* By default this is set to 120 secs. Setting this value to 0 will
	* turn off the timeout.
	*
	* @param timeoutSecs The timeout in secs
	*/
	void setUploadLowTransferRateTimeout(int32 timeoutSecs);

	/**
	* Returns the low transfer rate threshold in bytes/sec
	*
	* @returns The low transfer rate threshold in bytes/sec
	*/
	int32 getUploadLowTransferRateThreshold();

	/**
	* Sets the low transfer rate threshold of an upload in bytes/sec.
	* If the transfer rate dips below the given threshold longer
	* than the specified timeout, the transfer will fail.
	* By default this is set to 50 bytes/sec. Note that this setting
	* only works on platforms that use libcurl (non-windows and win32 but
	* not windows store or phone apps).
	*
	* @param bytesPerSec The low transfer rate threshold in bytes/sec
	*/
	void setUploadLowTransferRateThreshold(int32 bytesPerSec);

	/**
	* Enables the message caching upon network error, which is disabled by default.
	* Once enabled, if a client side network error is encountered
	* (i.e. brainCloud server is unreachable presumably due to the client
	* network being down) the sdk will do the following:
	*
	* 1 - cache the currently queued messages to brainCloud
	* 2 - call the network error callback
	* 3 - then expect the app to call either:
	*     a) retryCachedMessages() to retry sending to brainCloud
	*     b) flushCachedMessages() to dump all messages in the queue.
	*
	* Between steps 2 & 3, the app can prompt the user to retry connecting
	* to brainCloud to determine whether to follow path 3a or 3b.
	*
	* Note that if path 3a is followed, and another network error is encountered,
	* the process will begin all over again from step 1.
	*
	* WARNING - the brainCloud sdk will cache *all* api calls sent
	* when a network error is encountered if this mechanism is enabled.
	* This effectively freezes all communication with brainCloud.
	* Apps must call either retryCachedMessages() or flushCachedMessages()
	* for the brainCloud SDK to resume sending messages.
	* resetCommunication() will also clear the message cache.
	*
	* @param enabled True if message should be cached on timeout
	*/
	void enableNetworkErrorMessageCaching(bool enabled);

	/** Attempts to resend any cached messages. If no messages are in the cache,
	* this method does nothing.
	*/
	void retryCachedMessages();

	/**
	* Flushes the cached messages to resume api call processing. This will dump
	* all of the cached messages in the queue.
	*
	* @param sendApiErrorCallbacks If set to true API error callbacks will
	* be called for every cached message with statusCode CLIENT_NETWORK_ERROR
	* and reasonCode CLIENT_NETWORK_ERROR_TIMEOUT.
	*/
	void flushCachedMessages(bool sendApiErrorCallbacks);

	/**
	* Inserts a marker which will tell the brainCloud comms layer
	* to close the message bundle off at this point. Any messages queued
	* before this method was called will likely be bundled together in
	* the next send to the server.
	*
	* To ensure that only a single message is sent to the server you would
	* do something like this:
	*
	* InsertEndOfMessageBundleMarker()
	* SomeApiCall()
	* InsertEndOfMessageBundleMarker()
	*
	*/
	void insertEndOfMessageBundleMarker();

	/**
	* Sets the country code sent to brainCloud when a user authenticates.
	* Will override any auto detected country.
	* @param countryCode ISO 3166-1 two-letter country code
	*/
	void overrideCountryCode(const FString &countryCode) { 
		_country = UBrainCloudFunctionLibrary::FormatCountryCode(countryCode);
	}

	/**
	* Sets the language code sent to brainCloud when a user authenticates.
	* If the language is set to a non-ISO 639-1 standard value the game default will be used instead.
	* Will override any auto detected language.
	* @param languageCode ISO 639-1 two-letter language code
	*/
	void overrideLanguageCode(const FString &languageCode) { _language = languageCode; }

  protected:

	BrainCloudComms *_brainCloudComms = nullptr;
	BrainCloudRTTComms *_brainCloudRTTComms = nullptr;
	BrainCloudRelayComms *_brainCloudRelayComms = nullptr;

	BrainCloudAuthentication *_authenticationService = nullptr;
	BrainCloudLeaderboard *_leaderboardService = nullptr;
	BrainCloudPlayerState *_playerStateService = nullptr;
	BrainCloudGamification *_gamificationService = nullptr;
	BrainCloudGlobalEntity *_globalEntityService = nullptr;
	BrainCloudGlobalStatistics *_globalStatisticsService = nullptr;
	BrainCloudEntity *_entityService = nullptr;
	BrainCloudPlayerStatistics *_playerStatisticsService = nullptr;
	BrainCloudTime *_timeService = nullptr;
	BrainCloudPlayerStatisticsEvent *_playerStatisticsEventService = nullptr;
	BrainCloudIdentity *_identityService = nullptr;
	BrainCloudItemCatalog *_itemCatalogService = nullptr;
	BrainCloudUserItems *_userItemsService = nullptr;
	BrainCloudEvent *_eventService = nullptr;
	BrainCloudS3Handling *_s3HandlingService = nullptr;
	BrainCloudScript *_scriptService = nullptr;
	BrainCloudAsyncMatch *_asyncMatchService = nullptr;
	BrainCloudFriend *_friendService = nullptr;
	BrainCloudGlobalApp *_globalAppService = nullptr;
	BrainCloudMatchmaking *_matchmakingService = nullptr;
	BrainCloudOneWayMatch *_oneWayMatchService = nullptr;
	BrainCloudPlaybackStream *_playbackStreamService = nullptr;
	BrainCloudPushNotification *_pushNotificationService = nullptr;
	BrainCloudRedemptionCode *_redemptionCodeService = nullptr;
	BrainCloudDataStream *_dataStreamService = nullptr;
	BrainCloudProfanity *_profanityService = nullptr;
	BrainCloudFile *_fileService = nullptr;
	BrainCloudGroup *_groupService = nullptr;
	BrainCloudMail *_mailService = nullptr;
	BrainCloudTournament *_tournamentService = nullptr;
	BrainCloudGlobalFile *_globalFileService = nullptr;
	BrainCloudCustomEntity *_customEntityService = nullptr;
	BrainCloudPresence *_presenceService = nullptr;
	BrainCloudVirtualCurrency *_virtualCurrencyService = nullptr;
	BrainCloudAppStore *_appStoreService = nullptr;

	BrainCloudRTT *_rttService = nullptr;
	BrainCloudLobby *_lobbyService = nullptr;
	BrainCloudChat *_chatService = nullptr;
	BrainCloudMessaging *_messagingService = nullptr;

	BrainCloudRelay *_relayService = nullptr;

	BrainCloudTimeUtils *_brainCloudTimeUtils = nullptr;

	BrainCloudBlockchain *_blockchainService = nullptr;

	BrainCloudGroupFile *_groupFileService = nullptr;

	static FString s_brainCloudClientVersion;

	FString _appId = "";
	FString _releasePlatform = "";
	FString _appVersion = "";
	FString _country = "";
	FString _language = "";
	float _timezoneOffset = 0.0f;
	FString _sessionId = "";
	FString _profileId = "";

	void determineReleasePlatform();

	template <class T>
	void destroyService(T *&service);
};
