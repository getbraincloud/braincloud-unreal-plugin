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
     * Service Name - GlobalFileV3
     * Service Operation - GetFileInfo
     *
     * @param in_fileId The fileId of the global file
     * @param in_callback The method to be invoked when the server response is received
     */
    void getFileInfo(const FString &fileId, IServerCallback *callback);

    /**
     * Returns the complete info for the specified file, without having to look up the fileId first.
     *
     * Service Name - GlobalFileV3
     * Service Operation - GetFileInfoSimple
     *
     * @param in_folderPath The folder path of the file
     * @param in_filename The name of the file
     * @param in_callback The method to be invoked when the server response is received
     */
    void getFileInfoSimple(const FString &fileName, const FString &folderPath, IServerCallback *callback);

    /**
     * Returns the CDN of the specified file.
     *
     * Service Name - GlobalFileV3
     * Service Operation - GetGlobalCDNUrl
     *
     * @param in_fileId The fileId of the global file
     * @param in_callback The method to be invoked when the server response is received
     */
    void getGlobalCDNUrl(const FString &fileId, IServerCallback *callback);

    /**
     * Returns files at the current path.
     *
     * Service Name - GlobalFileV3
     * Service Operation - GetGlobalFileList
     *
     * @param in_folderPath The folder path to list files from
     * @param in_recurse Whether to recurse into subfolders
     * @param in_callback The method to be invoked when the server response is received
     */
    void getGlobalFileList(const FString &folderPath, bool recurse, IServerCallback *callback);

private:
    BrainCloudClient *_client = nullptr;
};