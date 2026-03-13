// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudEvent
{
public:
	BrainCloudEvent(BrainCloudClient *client);

	/**
	 * Sends an event to the designated user id with the attached json data.
	 * Any events that have been sent to a user will show up in their
	 * incoming event mailbox. If the recordLocally flag is set to true,
	 * a copy of this event (with the exact same event id) will be stored
	 * in the sending user's "sent" event mailbox.
	 *
	 * Note that the list of sent and incoming events for a user is returned
	 * in the "ReadPlayerState" call (in the BrainCloudPlayer module).
	 *
	 * Service Name - event
	 * Service Operation - SEND
	 *
	 * @param toProfileId The id of the user who is being sent the event
	 * @param eventType The user-defined type of the event.
	 * @param jsonEventData The user-defined data for this event encoded in JSON.
	 * @param callback The method to be invoked when the server response is received
	 */
	void sendEvent(const FString &toPlayerId, const FString &eventType, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Sends an event to multiple users with the attached json data.
	 *
	 * Service Name - event
	 * Service Operation - SEND_EVENT_TO_PROFILES
	 *
	 * @param toIds The profile ids of the users to send the event
	 * @param eventType The user-defined type of the event
	 * @param eventData The user-defined data for this event encoded in JSON
	 * @param callback The method to be invoked when the server response is received
	 */
	void sendEventToProfiles(const TArray<FString> &toIds, const FString &eventType, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Updates an event in the user's incoming event mailbox.
	 *
	 * Service Name - event
	 * Service Operation - UPDATE_EVENT_DATA
	 *
	 * @param evId The event id
	 * @param jsonEventData The user-defined data for this event encoded in JSON.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateIncomingEventData(const FString &evId, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Updates an event in the user's incoming event mailbox.
	 * Returns the same data as updateIncomingEventData, but returns null instead of an error if none exists.
	 *
	 * Service Name - event
	 * Service Operation - UPDATE_EVENT_DATA
	 *
	 * @param evId The event id
	 * @param jsonEventData The user-defined data for this event encoded in JSON.
	 * @param callback The method to be invoked when the server response is received
	 */
	void updateIncomingEventDataIfExists(const FString &evId, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Delete an event out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING
	 *
	 * @param evId The event id
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEvent(const FString &evId, IServerCallback *callback);

	/**
	 * Delete a list of events out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS
	 *
	 * @param eventIds Collection of event ids
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEvents(const TArray<FString> &eventIds, IServerCallback *callback);

	/**
	 * Delete any events older than the given date out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS_OLDER_THAN
	 *
	 * @param dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEventsOlderThan(int64 dateMillis, IServerCallback *callback);

	/**
	 * Delete any events of the given type older than the given date out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS_BY_TYPE_OLDER_THAN
	 *
	 * @param eventType The user-defined type of the event
	 * @param dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEventsByTypeOlderThan(const FString &eventType, int64 dateMillis, IServerCallback *callback);

	/**
	 * Get the events currently queued for the user.
	 *
	 * Service Name - event
	 * Service Operation - GET_EVENTS
	 *
	 * @param callback The method to be invoked when the server response is received
	 */
	void getEvents(IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
