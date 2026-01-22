// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;
class IAcl;

class BCCLIENTPLUGIN_API BrainCloudDataStream
{
public:
  BrainCloudDataStream(BrainCloudClient *client);

  /**
   * Creates custom data stream page event
   *
   * @param eventName Name of event
   * @param eventProperties Properties of event
   * @param in_callback The method to be invoked when the server response is received
   */
  void customPageEvent(const FString &eventName, const FString &jsonEventProperties, IServerCallback *callback = nullptr);

  /**
   * Creates custom data stream screen event
   *
   * @param eventName Name of event
   * @param eventProperties Properties of event
   * @param in_callback The method to be invoked when the server response is received
   */
  void customScreenEvent(const FString &eventName, const FString &jsonEventProperties, IServerCallback *callback = nullptr);

  /**
   * Creates custom data stream track event
   *
   * @param eventName Name of event
   * @param eventProperties Properties of event
   * @param in_callback The method to be invoked when the server response is received
   */
  void customTrackEvent(const FString &eventName, const FString &jsonEventProperties, IServerCallback *callback = nullptr);

  /**
   * Send crash report
   *
   * @param crashType
   * @param errorMsg
   * @param crashJson
   * @param crashLog
   * @param userName
   * @param userEmail
   * @param userNotes
   * @param userSubmitted
   * @param in_callback The method to be invoked when the server response is received
   */
  void submitCrashReport(const FString &crashType, const FString &errorMsg, const FString &crashJson, const FString &crashLog, const FString &userName, const FString &userEmail, const FString &userNotes, bool userSubmitted, IServerCallback *callback = nullptr);

private:
  BrainCloudClient *_client = nullptr;
};