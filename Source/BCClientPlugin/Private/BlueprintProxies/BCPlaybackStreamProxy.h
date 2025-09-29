// Copyright 2025 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCBlueprintCallProxyBase.h"
#include "BCPlaybackStreamProxy.generated.h"

UCLASS(MinimalAPI)
class UBCPlaybackStreamProxy : public UBCBlueprintCallProxyBase
{
    GENERATED_BODY()

  public:
    UBCPlaybackStreamProxy(const FObjectInitializer &ObjectInitializer);

    /**
    * Starts a stream
    *
    * Service Name - PlaybackStream
    * Service Operation - StartStream
    *
    * Param - targetPlayerId The player to start a stream with
    * Param - includeSharedData Whether to include shared data in the stream
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *StartStream(UBrainCloudWrapper *brainCloudWrapper, const FString &targetPlayerId, bool includeSharedData);

    /**
    * Reads a stream
    *
    * Service Name - PlaybackStream
    * Service Operation - ReadStream
    *
    * Param - playbackStreamId Identifies the stream to read
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *ReadStream(UBrainCloudWrapper *brainCloudWrapper, const FString &playbackStreamId);

    /**
    * Ends a stream
    *
    * Service Name - PlaybackStream
    * Service Operation - EndStream
    *
    * Param - playbackStreamId Identifies the stream to read
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *EndStream(UBrainCloudWrapper *brainCloudWrapper, const FString &playbackStreamId);

    /**
    * Deletes a stream
    *
    * Service Name - PlaybackStream
    * Service Operation - DeleteStream
    *
    * Param - playbackStreamId Identifies the stream to delete
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *DeleteStream(UBrainCloudWrapper *brainCloudWrapper, const FString &playbackStreamId);

    /**
    * Adds a stream event
    *
    * Service Name - PlaybackStream
    * Service Operation - AddEvent
    *
    * Param - playbackStreamId Identifies the stream to read
    * Param - jsonEventData Describes the event
    * Param - jsonSummary Current summary data as of this event
    */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *AddEvent(UBrainCloudWrapper *brainCloudWrapper, const FString &playbackStreamId, const FString &jsonEventData, const FString &jsonSummary);

    /**
	* Gets recent stream summaries for initiating player
	*
	* Service Name - PlaybackStream
	* Service Operation - GetRecentStreamsForInitiatingPlayer
	*
	* Param - initiatingPlayerId The player that started the stream
	* Param - maxNumStreams The max number of streams to query
	* Param - callback The callback.
	*/
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *GetRecentStreamsForInitiatingPlayer(UBrainCloudWrapper *brainCloudWrapper, const FString &initiatingPlayerId, int32 maxNumStreams);

    /**
	* Gets recent stream summaries for target player
	*
	* Service Name - PlaybackStream
	* Service Operation - GetRecentStreamsForTargetPlayer
	*
	* Param - targetPlayerId The player that was target of the stream
	* Param - maxNumStreams The max number of streams to query
	* Param - callback The callback.
	*/
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *GetRecentStreamsForTargetPlayer(UBrainCloudWrapper *brainCloudWrapper, const FString &targetPlayerId, int32 maxNumStreams);

    /**
     * Protects a playback stream from being purged (but not deleted) for the given number of days (from now).
     * If the number of days given is less than the normal purge interval days (from createdAt), the longer protection date is applied.
     * Can only be called by users involved in the playback stream.
     *
     * Service Name - PlaybackStream
     * Service Operation - PROTECT_STREAM_UNTIL
     *
     * @param playbackStreamId Identifies the stream to protect.
     * @param numDays The number of days the stream is to be protected (from now).
     * @param callback The callback.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|Playback Stream")
    static UBCPlaybackStreamProxy *ProtectStreamUntil(UBrainCloudWrapper* brainCloudWrapper, const FString& playbackStreamId, int32 numDays);
};
