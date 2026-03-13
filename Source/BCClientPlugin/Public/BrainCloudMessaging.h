// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudMessaging
{
public:
	BrainCloudMessaging(BrainCloudClient *client);

	/**
	 * Deletes specified user messages on the server.
	 *
	 * Service Name - messaging
	 * Service Operation - DELETE_MESSAGES
	 *
	 * @param msgbox The message box to delete from.
	 * @param msgIds Arrays of message ids to delete.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteMessages(const FString &in_msgBox, const TArray<FString> &in_msgsIds, IServerCallback *in_callback);

	/**
	 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
	 *
	 * Service Name - messaging
	 * Service Operation - GET_MESSAGE_BOXES
	 *
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void getMessageBoxes(IServerCallback *in_callback);

	/**
	 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
	 *
	 * Service Name - messaging
	 * Service Operation - GET_MESSAGE_COUNTS
	 *
	 * @param callback The method to be invoked when the server response is received
	 */
	void getMessageCounts(IServerCallback *in_callback);

	/**
	 * Retrieves list of specified messages.
	 *
	 * Service Name - messaging
	 * Service Operation - GET_MESSAGES
	 *
	 * @param msgbox The message box to get messages from.
	 * @param msgIds Arrays of message ids to get.
	 * @param markAsRead mark messages that are read
	 * @param callback The method to be invoked when the server response is received
	 */
	void getMessages(const FString &in_msgBox, const TArray<FString> &in_msgsIds, bool markAsRead, IServerCallback *in_callback);

	/**
	 * Retrieves a page of messages.
	 *
	 * Service Name - messaging
	 * Service Operation - GET_MESSAGES_PAGE
	 *
	 * @param context The context for the page of messages.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getMessagesPage(const FString &in_context, IServerCallback *in_callback);

	/**
	 * Gets the page of messages from the server based on the encoded context and specified page offset.
	 *
	 * Service Name - messaging
	 * Service Operation - GET_MESSAGES_PAGE_OFFSET
	 *
	 * @param context The context for the page of messages.
	 * @param pageOffset The page offset.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getMessagesPageOffset(const FString &in_context, int32 in_pageOffset, IServerCallback *in_callback);

	/**
	 * Marks list of user messages as read on the server.
	 *
	 * Service Name - messaging
	 * Service Operation - MARK_MESSAGES_READ
	 *
	 * @param msgbox The message box to mark as read.
	 * @param msgIds Arrays of message ids to mark as read.
	 * @param callback The method to be invoked when the server response is received
	 */
	void markMessagesRead(const FString &in_msgBox, const TArray<FString> &in_msgsIds, IServerCallback *in_callback);

	/**
	 * Sends a message with specified 'subject' and 'text' to list of users.
	 *
	 * Service Name - messaging
	 * Service Operation - SEND_MESSAGE
	 *
	 * @param toProfileIds The list of profile ids to send the message to.
	 * @param contentJson The message you are sending
	 * @param callback The method to be invoked when the server response is received
	 */
	void sendMessage(const TArray<FString> &in_toProfileIds, const FString &in_contentJson, IServerCallback *in_callback);

	/**
	 * Sends a simple message to specified list of users.
	 *
	 * Service Name - messaging
	 * Service Operation - SEND_MESSAGE_SIMPLE
	 *
	 * @param toProfileIds The list of profile ids to send the message to.
	 * @param messageText The message text you are sending
	 * @param callback The method to be invoked when the server response is received
	 */
	void sendMessageSimple(const TArray<FString> &in_toProfileIds, const FString &in_messageText, IServerCallback *in_callback);

private:
	BrainCloudClient *_client = nullptr;
};