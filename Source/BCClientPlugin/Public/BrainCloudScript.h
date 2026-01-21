// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudScript
{
public:
	BrainCloudScript(BrainCloudClient *client);

	/**
	 * Executes a script on the server.
	 *
	 * Service Name - Script
	 * Service Operation - Run
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_jsonScriptData Data to be sent to the script in json format
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void runScript(const FString &scriptName, const FString &jsonScriptData, IServerCallback *callback = nullptr);

	/**
	 * Allows cloud script executions to be scheduled - UTC time
	 *
	 * Service Name - Script
	 * Service Operation - ScheduleCloudScript
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_jsonScriptData Data to be sent to the script in json format
	 * @param in_startDateInUTC The start date in UTC
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void scheduleRunScriptMillisUTC(const FString &scriptName, const FString &jsonScriptData, int64 startDateInUTC, IServerCallback *callback = nullptr);

	/**
	 * Allows cloud script executions to be scheduled
	 *
	 * Service Name - Script
	 * Service Operation - ScheduleCloudScript
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_jsonScriptData Data to be sent to the script in json format
	 * @param in_minutesFromNow Number of minutes from now to run script
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void scheduleRunScriptMinutes(const FString &scriptName, const FString &jsonScriptData, int32 minutesFromNow, IServerCallback *callback = nullptr);

	/**
	 * Run a cloud script in a parent app
	 *
	 * Service Name - Script
	 * Service Operation - RUN_PARENT_SCRIPT
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_scriptData Data to be sent to the script in json format
	 * @param in_parentLevel The level name of the parent to run the script from
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void runParentScript(const FString &scriptName, const FString &jsonScriptData, const FString &parentLevel, IServerCallback *callback = nullptr);

	/**
	 * Cancels a scheduled cloud code script
	 *
	 * Service Name - Script
	 * Service Operation - CANCEL_SCHEDULED_SCRIPT
	 *
	 * @param in_jobId ID of script job to cancel
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void cancelScheduledScript(const FString &jobId, IServerCallback *callback = nullptr);

	/**
	 * Runs a script from the context of a peer
	 *
	 * Service Name - Script
	 * Service Operation - RUN_PEER_SCRIPT
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_jsonScriptData Data to be sent to the script in json format
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void runPeerScript(const FString &scriptName, const FString &jsonScriptData, const FString &peer, IServerCallback *callback = nullptr);

	/**
	 * Runs a script asynchronously from the context of a peer
	 * This method does not wait for the script to complete before returning
	 *
	 * Service Name - Script
	 * Service Operation - RUN_PEER_SCRIPT_ASYNC
	 *
	 * @param in_scriptName The name of the script to be run
	 * @param in_jsonScriptData Data to be sent to the script in json format
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void runPeerScriptAsync(const FString &scriptName, const FString &jsonScriptData, const FString &peer, IServerCallback *callback = nullptr);

	/**
	 * Cancels a scheduled cloud code script
	 *
	 * Service Name - Script
	 * Service Operation - CANCEL_SCHEDULED_SCRIPT
	 *
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getRunningOrQueuedCloudScripts(IServerCallback *callback = nullptr);

	/**
	 * Cancels a scheduled cloud code script
	 *
	 * Service Name - Script
	 * Service Operation - CANCEL_SCHEDULED_SCRIPT
	 *
	 * @param in_jobId ID of script job to cancel
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void getScheduledCloudScripts(const struct FDateTime startDateInUTC, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
};