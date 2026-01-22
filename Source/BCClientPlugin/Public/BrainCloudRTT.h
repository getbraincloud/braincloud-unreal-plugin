// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

enum class BCRTTConnectionStatus : uint8;
enum class BCRTTConnectionType : uint8;
class BrainCloudClient;
class BrainCloudRTTComms;
class UBCBlueprintRTTCallProxyBase;
class IRTTCallback;
class IServerCallback;
class UBCRTTProxy;

class BCCLIENTPLUGIN_API BrainCloudRTT
{
public:
	BrainCloudRTT(BrainCloudRTTComms *comms, BrainCloudClient *client);

	/// <summary>
	/// Requests the event server address
	/// </summary>
	void requestClientConnection(IServerCallback *callback);

	/**
	 * Enables Real Time event for this session.
	 * Real Time events are disabled by default. Usually events
	 * need to be polled using GET_EVENTS. By enabling this, events will
	 * be received instantly when they happen through a TCP connection to an Event Server.
	 *
	 * This function will first call requestClientConnection, then connect to the address
	 *
	 * @param callback The callback.
	 * @param useWebSocket Use web sockets instead of TCP for the internal connections. Default is true
	 */
	void enableRTT(BCRTTConnectionType in_type, IServerCallback *in_callback);

	/*
	 * Enables Real Time event for this session.
	 * Real Time events are disabled by default. Usually events
	 * need to be polled using GET_EVENTS. By enabling this, events will
	 * be received instantly when they happen through a TCP connection to an Event Server.
	 *
	 * This function will first call requestClientConnection, then connect to the address
	 */
	void enableRTT(BCRTTConnectionType in_type, UBCRTTProxy *in_callback);

	/*
	 * Disables Real Time event for this session.
	 */
	void disableRTT();

	/*
	 *Returns true id RTT is enabled
	 */
	bool isRTTEnabled();

	/**
	 * Listen to real time events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one event callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void setRTTHeartBeatSeconds(int32 in_value);

	/**
	 * Clear all set RTT callbacks
	 */
	void deregisterAllRTTCallbacks();

	/**
	 * Listen to real time events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one event callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void registerRTTEventCallback(UBCBlueprintRTTCallProxyBase *in_callback);

	/**
		 * Listen to real time messaging.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one messaging callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
	void registerRTTEventCallback(IRTTCallback *in_callback);

	/**
	 * Listen to real time presence events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void deregisterRTTEventCallback();

	/**
	 * Listen to real time chat messages.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one chat callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void registerRTTChatCallback(UBCBlueprintRTTCallProxyBase *in_callback);

	/**
	 * Clear all set RTT callbacks
	 */
	void registerRTTChatCallback(IRTTCallback *in_callback);

	/**
	 * Listen to real time messaging.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one messaging callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void deregisterRTTChatCallback();

	/**
	 * Listen to real time lobby events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one lobby callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void registerRTTMessagingCallback(UBCBlueprintRTTCallProxyBase *in_callback);

	/**
	 * Listen to real time presence events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void registerRTTMessagingCallback(IRTTCallback *in_callback);

	/**
	 * Listen to real time blockchain events.
	 *
	 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
	 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
	 */
	void deregisterRTTMessagingCallback();

	/**
		 * Listen to real time lobby events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one lobby callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
	void registerRTTPresenceCallback(UBCBlueprintRTTCallProxyBase *in_callback);

	/**
		 * Listen to real time presence events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
	void registerRTTPresenceCallback(IRTTCallback *in_callback);

	/**
		 * Listen to real time blockchain events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
	void deregisterRTTPresenceCallback();
	/**
	 *
	 */
	void registerRTTLobbyCallback(UBCBlueprintRTTCallProxyBase *in_callback);

	/**
	 *
	 */
	void registerRTTLobbyCallback(IRTTCallback *in_callback);

	/**
	 *
	 */
	void deregisterRTTLobbyCallback();

	/**
	 *
	 */
	void registerRTTBlockchainRefresh(IRTTCallback *in_callback);

	/**
	 *
	 */
	void deregisterRTTBlockchainRefresh();

	const FString &getRTTConnectionId() const;
	BCRTTConnectionStatus getConnectionStatus();

private:
	BrainCloudClient *_client = nullptr;
	BrainCloudRTTComms *_commsLayer = nullptr;
};
