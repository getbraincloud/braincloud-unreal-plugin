// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudChat
{
public:
	BrainCloudChat(BrainCloudClient *client);

	/**
	 * Registers a listener for incoming events from <channelId>.
	 * Also returns a list of <maxReturn> recent messages from history.
	 *
	 * Service Name - Chat
	 * Service Operation - ChannelConnect
	 *
	 * @param channelId The id of the chat channel to return history from.
	 * @param maxReturn Maximum number of messages to return.
	 * @param callback The method to be invoked when the server response is received
	 */
	void channelConnect(const FString &in_channelId, int32 in_maxToReturn, IServerCallback *in_callback);

	/**
	 * Unregisters a listener for incoming events from <channelId>.
	 *
	 * Service Name - Chat
	 * Service Operation - ChannelDisconnect
	 *
	 * @param channelId The id of the chat channel to unsubscribed from.
	 * @param callback The method to be invoked when the server response is received
	 */
	void channelDisconnect(const FString &in_channelId, IServerCallback *in_callback);

	/**
	 * Delete a chat message. <version> must match the latest or pass -1 to bypass version check.
	 *
	 * Service Name - Chat
	 * Service Operation - DeleteChatMessage
	 *
	 * @param channelId The id of the chat channel that contains the message to delete.
	 * @param msgId The message id to delete.
	 * @param version Version of the message to delete. Must match latest or pass -1 to bypass version check.
	 * @param callback The method to be invoked when the server response is received
	 */
	void deleteChatMessage(const FString &in_channelId, const FString &in_messageId, int32 in_version, IServerCallback *in_callback);

	/**
	 * Gets the channelId for the given <channelType> and <channelSubId>. Channel type must be one of "gl" or "gr".
	 *
	 * Service Name - Chat
	 * Service Operation - GetChannelId
	 *
	 * @param channelType Channel type must be one of "gl" or "gr". For (global) or (group) respectively.
	 * @param channelSubId The sub id of the channel.
	 * @param callback The method to be invoked when the server response is received
	 */
	void getChannelId(const FString &in_channelType, const FString &in_channelSubId, IServerCallback *in_callback);

	/**
	 * Gets description info and activity stats for channel <channelId>.
	 * Note that numMsgs and listeners only returned for non-global groups.
	 * Only callable for channels the user is a member of.
	 *
	 * Service Name - Chat
	 * Service Operation - GetChannelInfo
	 *
	 * @param channelId Id of the channel to receive the info from.
	 * @param callback The method to be invoked when the server response is received.
	 */
	void getChannelInfo(const FString &in_channelId, IServerCallback *in_callback);

	/**
	 * Gets a populated chat object (normally for editing).
	 *
	 * Service Name - Chat
	 * Service Operation - GetChatMessage
	 *
	 * @param channelId Id of the channel to receive the message from.
	 * @param msgId Id of the message to read.
	 * @param callback The method to be invoked when the server response is received.
	 */
	void getChatMessage(const FString &in_channelId, const FString &in_messageId, IServerCallback *in_callback);

	/**
	 * Get a list of <maxReturn> messages from history of channel <channelId>.
	 *
	 * Service Name - Chat
	 * Service Operation - GetRecentChatMessages
	 *
	 * @param channelId Id of the channel to receive the info from.
	 * @param maxReturn Maximum message count to return.
	 * @param callback The method to be invoked when the server response is received.
	 */
	void getRecentChatMessages(const FString &in_channelId, int32 in_maxToReturn, IServerCallback *in_callback);

	/**
	 * Gets a list of the channels of type <channelType> that the user has access to.
	 * Channel type must be one of "gl", "gr" or "all".
	 *
	 * Service Name - Chat
	 * Service Operation - GetSubscribedChannels
	 *
	 * @param channelType Type of channels to get back. "gl" for global, "gr" for group or "all" for both.
	 * @param callback The method to be invoked when the server response is received.
	 */
	void getSubscribedChannels(const FString &in_channelType, IServerCallback *in_callback);

	/**
	 * Send a potentially rich chat message.
	 *
	 * Service Name - chat
	 * Service Operation - POST_CHAT_MESSAGE
	 *
	 * @param in_channelId The channelId to post the chat message to
	 * @param in_plain The plain text message asscociated with the chat message
	 * @param in_jsonRich The json string to associate with the chat message
	 * @param in_recordInHistory Whether to record the message in history, or to only send it to currently active participants
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void postChatMessage(const FString &in_channelId, const FString &in_plain, const FString &in_jsonRich, bool in_recordInHistory, IServerCallback *in_callback);

	/**
	 * Send a chat message with text only
	 *
	 * Service Name - Chat
	 * Service Operation - PostChatMessage
	 *
	 * @param channelId Channel id to post message to.
	 * @param text The text message.
	 * @param callback The method to be invoked when the server response is received.
	 */
	void postChatMessageSimple(const FString &in_channelId, const FString &in_plain, bool in_recordInHistory, IServerCallback *in_callback);

	/**
	 * Update a chat message. <content> must contain at least a "plain" field for plain-text messaging. <version> must match the latest or pass -1 to bypass version check.
	 *
	 * Service Name - chat
	 * Service Operation - UPDATE_CHAT_MESSAGE
	 *
	 * @param in_channelId The channelId of the chat message to update
	 * @param in_messageId The messagedId of the message to update
	 * @param in_version The version of the message to update, must match the latest or pass -1 to bypass version check.
	 * @param in_plain The plain text message asscociated with the chat message
	 * @param in_jsonRich The json string to associate with the chat message
	 * @param in_recordInHistory Whether to record the message in history, or to only send it to currently active participants
	 * @param in_callback Method to be invoked when the server response is received.
	 */
	void updateChatMessage(const FString &in_channelId, const FString &in_messageId, int32 in_version,
						   const FString &in_plain, const FString &in_jsonRich, IServerCallback *in_callback);

private:
	BrainCloudClient *_client = nullptr;
};
