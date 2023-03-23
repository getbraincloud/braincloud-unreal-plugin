#pragma once
#include "BrainCloudGroupACL.h"
#include "ServiceOperation.h"

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudGroupFile
{
public:
	BrainCloudGroupFile(BrainCloudClient *in_client);

	void checkFilenameExists(FString in_groupId, FString in_folderPath, FString in_filename, IServerCallback *in_callback);

	void checkFullpathFilenameExists(FString in_groupId, FString in_fullPathFilename, IServerCallback *in_callback);

	void copyFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void deleteFile(FString in_groupId, FString in_fileId, int in_version, FString in_filename, IServerCallback *in_callback);

	void getCDNUrl(FString in_groupId, FString in_fileId, IServerCallback *in_callback);

	void getFileInfo(FString in_groupId, FString in_fileId, IServerCallback *in_callback);

	void getFileInfoSimple(FString in_groupId, FString in_folderPath,FString in_filename, IServerCallback *in_callback);

	void getFileList(FString in_groupId, FString in_folderPath, bool in_recurse, IServerCallback *in_callback);

	void moveFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void moveUserToGroupFile(FString in_userCloudPath, FString in_userCloudFilename, FString in_groupId, FString in_groupTreeId, FString in_groupFilename, UBrainCloudGroupACL* in_groupFileAclJson, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void updateFileInfo(FString in_groupId, FString in_fileId, int in_version, FString in_newFilename, UBrainCloudGroupACL* in_newAclJson, IServerCallback *in_callback);

	
private:
	void sendRequest(ServiceOperation in_operation, TSharedRef<FJsonObject> in_data, IServerCallback *in_callback);

	BrainCloudClient *_client = nullptr;
};
