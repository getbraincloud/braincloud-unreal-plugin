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
	 * incoming event mailbox. If the in_recordLocally flag is set to true,
	 * a copy of this event (with the exact same event id) will be stored
	 * in the sending user's "sent" event mailbox.
	 *
	 * Note that the list of sent and incoming events for a user is returned
	 * in the "ReadPlayerState" call (in the BrainCloudPlayer module).
	 *
	 * Service Name - event
	 * Service Operation - SEND
	 *
	 * @param in_toProfileId The id of the user who is being sent the event
	 * @param in_eventType The user-defined type of the event.
	 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendEvent(const FString &toPlayerId, const FString &eventType, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Sends an event to multiple users with the attached json data.
	 *
	 * Service Name - Event
	 * Service Operation - SEND_EVENT_TO_PROFILES
	 *
	 * @param in_toIds The profile ids of the users to send the event
	 * @param in_eventType The user-defined type of the event
	 * @param in_eventData The user-defined data for this event encoded in JSON
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendEventToProfiles(const TArray<FString> &toIds, const FString &eventType, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Updates an event in the user's incoming event mailbox.
	 *
	 * Service Name - event
	 * Service Operation - UPDATE_EVENT_DATA
	 *
	 * @param in_evId The event id
	 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateIncomingEventData(const FString &evId, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Updates an event in the user's incoming event mailbox.
	 * Returns the same data as updateIncomingEventData, but returns null instead of an error if none exists.
	 *
	 * Service Name - event
	 * Service Operation - UPDATE_EVENT_DATA
	 *
	 * @param in_evId The event id
	 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void updateIncomingEventDataIfExists(const FString &evId, const FString &jsonEventData, IServerCallback *callback);

	/**
	 * Delete an event out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING
	 *
	 * @param in_evId The event id
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEvent(const FString &evId, IServerCallback *callback);

	/**
	 * Delete a list of events out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS
	 *
	 * @param in_eventIds Collection of event ids
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEvents(const TArray<FString> &eventIds, IServerCallback *callback);

	/**
	 * Delete any events older than the given date out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS_OLDER_THAN
	 *
	 * @param in_dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEventsOlderThan(int64 dateMillis, IServerCallback *callback);

	/**
	 * Delete any events of the given type older than the given date out of the user's incoming mailbox.
	 *
	 * Service Name - event
	 * Service Operation - DELETE_INCOMING_EVENTS_BY_TYPE_OLDER_THAN
	 *
	 * @param in_eventType The user-defined type of the event
	 * @param in_dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void deleteIncomingEventsByTypeOlderThan(const FString &eventType, int64 dateMillis, IServerCallback *callback);

	/**
	 * Get the events currently queued for the user.
	 *
	 * Service Name - event
	 * Service Operation - GET_EVENTS
	 *
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getEvents(IServerCallback *callback);

private:
	BrainCloudClient *_client = nullptr;
};
