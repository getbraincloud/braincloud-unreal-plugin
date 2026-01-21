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
	 * @param callback The method to be invoked when the server response is received
	 */
	void forcePush(IServerCallback *callback);

	/**
	 * Gets the presence data for the given <platform>. Can be one of "all",
	 * "brainCloud", or "facebook". Will not include offline profiles
	 * unless <includeOffline> is set to true.
	 */
	void getPresenceOfFriends(const FString &platform, bool includeOffline, IServerCallback *callback);

	/**
	 * Gets the presence data for the given <groupId>. Will not include
	 * offline profiles unless <includeOffline> is set to true.
	 */
	void getPresenceOfGroup(const FString &groupId, bool includeOffline, IServerCallback *callback);

	/**
		 * Gets the presence data for the given <profileIds>. Will not include
		 * offline profiles unless <includeOffline> is set to true.
		 */
	void getPresenceOfUsers(const TArray<FString> &profileIds, bool includeOffline, IServerCallback *callback);

	/**
	 * Registers the caller for RTT presence updates from friends for the
	 * given <platform>. Can be one of "all", "brainCloud", or "facebook".
	 * If <bidirectional> is set to true, then also registers the targeted
	 * users for presence updates from the caller.
	 */
	void registerListenersForFriends(const FString &platform, bool biderectional, IServerCallback *callback);

	/**
	 * Registers the caller for RTT presence updates from the members of
	 * the given <groupId>. Caller must be a member of said group. If
	 * <bidirectional> is set to true, then also registers the targeted
	 * users for presence updates from the caller.
	 */
	void registerListenersForGroup(const FString &groupId, bool biderectional, IServerCallback *callback);

	/**
		 * Registers the caller for RTT presence updates for the given
		 * <profileIds>. If <bidirectional> is set to true, then also registers
		 * the targeted users for presence updates from the caller.
		 */
	void registerListenersForProfiles(const TArray<FString> &profileIds, bool bidirectional, IServerCallback *callback);

	/**
	 * Update the presence data visible field for the caller.
	 */
	void setVisibility(bool visible, IServerCallback *callback);

	/**
	 * Stops the caller from receiving RTT presence updates. Does not
	 * affect the broadcasting of *their* presence updates to other
	 * listeners.
	 */
	void stopListening(IServerCallback *callback);

	/**
	 * Update the presence data activity field for the caller.
	 */
	void updateActivity(const FString &activity, IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
