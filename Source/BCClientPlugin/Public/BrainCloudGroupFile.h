#pragma once
#include "BrainCloudGroupACL.h"
#include "ServiceOperation.h"

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudGroupFile
{
public:
	BrainCloudGroupFile(BrainCloudClient *in_client);

	void CheckFilenameExists(FString in_groupId, FString in_folderPath, FString in_filename, IServerCallback *in_callback);

	void CheckFullpathFilenameExists(FString in_groupId, FString in_fullPathFilename, IServerCallback *in_callback);

	void CopyFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void DeleteFile(FString in_groupId, FString in_fileId, int in_version, FString in_filename, IServerCallback *in_callback);

	void GetCDNUrl(FString in_groupId, FString in_fileId, IServerCallback *in_callback);

	void GetFileInfo(FString in_groupId, FString in_fileId, IServerCallback *in_callback);

	void GetFileInfoSimple(FString in_groupId, FString in_folderPath,FString in_filename, IServerCallback *in_callback);

	void GetFileList(FString in_groupId, FString in_folderPath, bool in_recurse, IServerCallback *in_callback);

	void MoveFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void MoveUserToGroupFile(FString in_userCloudPath, FString in_userCloudFilename, FString in_groupId, FString in_groupTreeId, FString in_groupFilename, UBrainCloudGroupACL* in_groupFileAclJson, bool in_overwriteIfPresent, IServerCallback *in_callback);

	void UpdateFileInfo(FString in_groupId, FString in_fileId, int in_version, FString in_newFilename, UBrainCloudGroupACL* in_newAclJson, IServerCallback *in_callback);

	
private:
	void SendRequest(ServiceOperation in_operation, TSharedRef<FJsonObject> in_data, IServerCallback *in_callback);

	BrainCloudClient *_client = nullptr;
};
