// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudGlobalFile
{
public:
     BrainCloudGlobalFile(BrainCloudClient *client);

     /**
      * Returns the complete info for the specified file given it’s fileId
      *
      * Service Name - globalFileV3
      * Service Operation - GET_FILE_INFO
      *
      * @param fileId The fileId of the global file
      * @param callback The method to be invoked when the server response is received
      */
     void getFileInfo(const FString &fileId, IServerCallback *callback);

     /**
      * Returns the complete info for the specified file, without having to look up the fileId first.
      *
      * Service Name - globalFileV3
      * Service Operation - GET_FILE_INFO_SIMPLE
      *
      * @param folderPath The folder path of the file
      * @param filename The name of the file
      * @param callback The method to be invoked when the server response is received
      */
     void getFileInfoSimple(const FString &fileName, const FString &folderPath, IServerCallback *callback);

     /**
      * Returns the CDN of the specified file.
      *
      * Service Name - globalFileV3
      * Service Operation - GET_GLOBAL_CDN_URL
      *
      * @param fileId The fileId of the global file
      * @param callback The method to be invoked when the server response is received
      */
     void getGlobalCDNUrl(const FString &fileId, IServerCallback *callback);

     /**
      * Returns files at the current path.
      *
      * Service Name - globalFileV3
      * Service Operation - GET_GLOBAL_FILE_LIST
      *
      * @param folderPath The folder path to list files from
      * @param recurse Whether to recurse into subfolders
      * @param callback The method to be invoked when the server response is received
      */
     void getGlobalFileList(const FString &folderPath, bool recurse, IServerCallback *callback);

private:
     BrainCloudClient *_client = nullptr;
};