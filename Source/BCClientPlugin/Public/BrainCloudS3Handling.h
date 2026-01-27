// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudS3Handling
{
public:
       BrainCloudS3Handling(BrainCloudClient *client);

       /**
        * Sends an array of file details and returns
        * the details of any of those files that have changed
        *
        * Service Name - s3Handling
        * Service Operation - GET_UPDATED_FILES
        *
        * @param category  Category of files on server to compare against
        * @param fileDetailsJson  An array of file details
        * @param callback  Instance of IServerCallback to call when the server response is received
        */
       void getUpdatedFiles(const FString &category, const FString &fileDetails, IServerCallback *callback = nullptr);

       /**
        * Retrieves the details of custom files stored on the server
        *
        * Service Name - s3Handling
        * Service Operation - GET_FILE_LIST
        *
        * @param category  Category of files to retrieve
        * @param callback  Instance of IServerCallback to call when the server response is receieved
        */
       void getFileList(const FString &category, IServerCallback *callback = nullptr);

       /**
        * Returns the CDN url for a file
        *
        * Service Name - s3Handling
        * Service Operation - GET_CDN_URL
        *
        * @param fileId ID of file
        * @param callback The method to be invoked when the server response is received
        */
       void getCDNUrl(const FString &fileId, IServerCallback *callback = nullptr);

private:
       BrainCloudClient *_client = nullptr;
};
