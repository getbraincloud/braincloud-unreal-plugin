// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudPresence
{
public:
	BrainCloudPresence(BrainCloudClient *client);

	/**
	 * Force an RTT presence update to all listeners of the caller.
	 *
	 * Service Name - Presence
	 * Service Operation - ForcePush
	 *
	 * @param in_callback The callback invoked when the server response is received.
	 */
	void forcePush(IServerCallback *callback);

	/**
	 * Retrieves the presence data for friends on the specified platform.
	 *
	 * @param in_platform One of "all", "brainCloud", or "facebook".
	 * @param in_includeOffline If true, includes offline profiles.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void getPresenceOfFriends(const FString &platform, bool includeOffline, IServerCallback *callback);

	/**
	 * Retrieves the presence data for members of a given group.
	 *
	 * @param in_groupId Group ID to query.
	 * @param in_includeOffline If true, includes offline profiles.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void getPresenceOfGroup(const FString &groupId, bool includeOffline, IServerCallback *callback);

	/**
	 * Retrieves the presence data for the specified users.
	 *
	 * @param in_profileIds Vector of profile IDs to query.
	 * @param in_includeOffline If true, includes offline profiles.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void getPresenceOfUsers(const TArray<FString> &profileIds, bool includeOffline, IServerCallback *callback);

	/**
	 * Registers the caller for RTT presence updates from friends on a given platform.
	 *
	 * @param in_platform One of "all", "brainCloud", or "facebook".
	 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void registerListenersForFriends(const FString &platform, bool biderectional, IServerCallback *callback);

	/**
	 * Registers the caller for RTT presence updates from members of a given group.
	 *
	 * @param in_groupId Group ID to listen to. Caller must be a member.
	 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void registerListenersForGroup(const FString &groupId, bool biderectional, IServerCallback *callback);

	/**
	 * Registers the caller for RTT presence updates from specific profiles.
	 *
	 * @param in_profileIds Vector of profile IDs to listen to.
	 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void registerListenersForProfiles(const TArray<FString> &profileIds, bool bidirectional, IServerCallback *callback);

	/**
	 * Updates the visibility field of the caller's presence data.
	 *
	 * @param in_visible True to make the caller visible, false to hide.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void setVisibility(bool visible, IServerCallback *callback);

	/**
	 * Stops the caller from receiving RTT presence updates.
	 * Does not affect broadcasting of the caller's own presence updates.
	 *
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void stopListening(IServerCallback *callback);

	/**
	 * Updates the activity field of the caller's presence data.
	 *
	 * @param in_jsonActivity JSON string representing activity information.
	 * @param in_callback Callback invoked when the server response is received.
	 */
	void updateActivity(const FString &activity, IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
