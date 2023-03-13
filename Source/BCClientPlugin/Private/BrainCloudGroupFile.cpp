#include "BrainCloudGroupFile.h"

#include "BrainCloudClient.h"
#include "JsonUtil.h"
#include "OperationParam.h"
#include "ServerCall.h"

BrainCloudGroupFile::BrainCloudGroupFile(BrainCloudClient* in_client) : _client(in_client){}

void BrainCloudGroupFile::CheckFilenameExists(FString in_groupId, FString in_folderPath, FString in_filename,
	IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FolderPath.getValue(), in_folderPath);
	data->SetStringField(OperationParam::FileName.getValue(), in_filename);
	
	SendRequest(ServiceOperation::CheckFilenameExists, data, in_callback);
}

void BrainCloudGroupFile::CheckFullpathFilenameExists(FString in_groupId, FString in_fullPathFilename,
	IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FullPathFilename.getValue(), in_fullPathFilename);
	
	SendRequest(ServiceOperation::CheckFullpathFilenameExists, data, in_callback);
}

void BrainCloudGroupFile::CopyFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId,
	int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);
	data->SetNumberField(OperationParam::Version.getValue(), in_version);
	data->SetStringField(OperationParam::NewTreeId.getValue(), in_newTreeId);
	data->SetNumberField(OperationParam::TreeVersion.getValue(), in_treeVersion);
	data->SetStringField(OperationParam::NewFilename.getValue(), in_newFilename);
	data->SetBoolField(OperationParam::OverwriteIfPresent.getValue(), in_overwriteIfPresent);

	SendRequest(ServiceOperation::CopyGroupFile, data, in_callback);
}

void BrainCloudGroupFile::DeleteFile(FString in_groupId, FString in_fileId, int in_version, FString in_filename,
	IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());
	
	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);
	data->SetNumberField(OperationParam::Version.getValue(), in_version);
	data->SetStringField(OperationParam::FileName.getValue(), in_filename);

	SendRequest(ServiceOperation::DeleteGroupFile, data, in_callback);
}

void BrainCloudGroupFile::GetCDNUrl(FString in_groupId, FString in_fileId, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);

	SendRequest(ServiceOperation::GetCdnUrl, data, in_callback);
}

void BrainCloudGroupFile::GetFileInfo(FString in_groupId, FString in_fileId, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);

	SendRequest(ServiceOperation::GetFileInfo, data, in_callback);
}

void BrainCloudGroupFile::GetFileInfoSimple(FString in_groupId, FString in_folderPath, FString in_filename,
	IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());
	
	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FolderPath.getValue(), in_folderPath);
	data->SetStringField(OperationParam::FileName.getValue(), in_filename);

	SendRequest(ServiceOperation::GetFileInfoSimple, data, in_callback);
}

void BrainCloudGroupFile::GetFileList(FString in_groupId, FString in_folderPath, bool in_recurse,
	IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FolderPath.getValue(), in_folderPath);
	data->SetBoolField(OperationParam::Recurse.getValue(), in_recurse);

	SendRequest(ServiceOperation::GetFileList, data, in_callback);
}

void BrainCloudGroupFile::MoveFile(FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId,
	int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);
	data->SetNumberField(OperationParam::Version.getValue(), in_version);
	data->SetStringField(OperationParam::NewTreeId.getValue(), in_newTreeId);
	data->SetNumberField(OperationParam::TreeVersion.getValue(), in_treeVersion);
	data->SetStringField(OperationParam::NewFilename.getValue(), in_newFilename);
	data->SetBoolField(OperationParam::OverwriteIfPresent.getValue(), in_overwriteIfPresent);

	SendRequest(ServiceOperation::MoveGroupFile, data, in_callback);
}

void BrainCloudGroupFile::MoveUserToGroupFile(FString in_userCloudPath, FString in_userCloudFilename,
	FString in_groupId, FString in_groupTreeId, FString in_groupFilename, UBrainCloudGroupACL* in_groupFileAclJson,
	bool in_overwriteIfPresent, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::UserCloudPath.getValue(), in_userCloudPath);
	data->SetStringField(OperationParam::UserCloudFilename.getValue(), in_userCloudFilename);
	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::GroupTreeId.getValue(), in_groupTreeId);
	data->SetStringField(OperationParam::GroupFilename.getValue(), in_groupFilename);
	data->SetObjectField(OperationParam::GroupFileACL.getValue(), in_groupFileAclJson->toJsonObject());
	data->SetBoolField(OperationParam::OverwriteIfPresent.getValue(), in_overwriteIfPresent);

	SendRequest(ServiceOperation::MoveUserToGroupFile, data, in_callback);
}

void BrainCloudGroupFile::UpdateFileInfo(FString in_groupId, FString in_fileId, int in_version, FString in_newFilename,
	UBrainCloudGroupACL* in_newAclJson, IServerCallback* in_callback)
{
	TSharedRef<FJsonObject> data = MakeShareable(new FJsonObject());

	data->SetStringField(OperationParam::GroupId.getValue(), in_groupId);
	data->SetStringField(OperationParam::FileId.getValue(), in_fileId);
	data->SetNumberField(OperationParam::Version.getValue(), in_version);
	data->SetStringField(OperationParam::NewFilename.getValue(), in_newFilename);
	data->SetObjectField(OperationParam::NewACL.getValue(), in_newAclJson->toJsonObject());

	SendRequest(ServiceOperation::UpdateFileInfo, data, in_callback);
}

void BrainCloudGroupFile::SendRequest(ServiceOperation in_operation, TSharedRef<FJsonObject> in_data,IServerCallback* in_callback)
{
	ServerCall *serverCall = new ServerCall(ServiceName::GroupFile, in_operation, in_data, in_callback);
	_client->sendRequest(serverCall);
}
