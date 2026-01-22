// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudPlaybackStream
{
public:
      BrainCloudPlaybackStream(BrainCloudClient *client);

      /**
       * Starts a stream
       *
       * Service Name - PlaybackStream
       * Service Operation - StartStream
       *
       * @param in_targetPlayerId The player to start a stream with
       * @param in_includeSharedData Whether to include shared data in the stream
       * @param in_callback The method to be invoked when the server response is received
       */
      void startStream(const FString &targetPlayerId, bool includeSharedData, IServerCallback *callback);

      /**
       * Reads a stream
       *
       * Service Name - PlaybackStream
       * Service Operation - ReadStream
       *
       * @param in_playbackStreamId Identifies the stream to read
       * @param in_callback The method to be invoked when the server response is received
       */
      void readStream(const FString &playbackStreamId, IServerCallback *callback);

      /**
       * Ends a stream
       *
       * Service Name - PlaybackStream
       * Service Operation - EndStream
       *
       * @param in_playbackStreamId Identifies the stream to read
       * @param in_callback The method to be invoked when the server response is received
       */
      void endStream(const FString &playbackStreamId, IServerCallback *callback);

      /**
       * Deletes a stream
       *
       * Service Name - PlaybackStream
       * Service Operation - DeleteStream
       *
       * @param in_playbackStreamId Identifies the stream to read
       * @param in_callback The method to be invoked when the server response is received
       */
      void deleteStream(const FString &playbackStreamId, IServerCallback *callback);

      /**
       * Adds a stream event
       *
       * Service Name - PlaybackStream
       * Service Operation - AddEvent
       *
       * @param in_playbackStreamId Identifies the stream to read
       * @param in_jsonEventData Describes the event
       * @param in_jsonSummary Current summary data as of this event
       * @param in_callback The method to be invoked when the server response is received
       */
      void addEvent(const FString &playbackStreamId, const FString &jsonEventData, const FString &jsonSummary, IServerCallback *callback);

      /**
       * Gets recent stream summaries for initiating player
       *
       * Service Name - PlaybackStream
       * Service Operation - GetRecentStreamsForInitiatingPlayer
       *
       * @param targetPlayerId The player that started the stream
       * @param maxNumStreams The max number of streams to query
       * @param callback The callback.
       */
      void getRecentStreamsForInitiatingPlayer(const FString &initiatingPlayerId, int32 maxNumStreams, IServerCallback *callback);

      /**
       * Gets recent stream summaries for target player
       *
       * Service Name - PlaybackStream
       * Service Operation - GetRecentStreamsForTargetPlayer
       *
       * @param targetPlayerId The player that was target of the stream
       * @param maxNumStreams The max number of streams to query
       * @param callback The callback.
       */
      void getRecentStreamsForTargetPlayer(const FString &targetPlayerId, int32 maxNumStreams, IServerCallback *callback);

      /**
       * Protects a playback stream from being purged (but not deleted) for the given number of days (from now).
       * If the number of days given is less than the normal purge interval days (from createdAt), the longer protection date is applied.
       * Can only be called by users involved in the playback stream.
       *
       * Service Name - PlaybackStream
       * Service Operation - PROTECT_STREAM_UNTIL
       *
       * @param in_playbackStreamId Identifies the stream to protect
       * @param in_numDays The number of days the stream is to be protected (from now)
       * @param in_callback The method to be invoked when the server response is received
       */
      void protectStreamUntil(const FString &playbackStreamId, int32 numDays, IServerCallback *callback);

private:
      BrainCloudClient *_client = nullptr;
};
