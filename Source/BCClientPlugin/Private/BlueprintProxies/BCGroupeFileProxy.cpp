#include "BCGroupeFileProxy.h"

#include "BCWrapperProxy.h"
#include "BrainCloudWrapper.h"

UBCGroupeFileProxy::UBCGroupeFileProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBCGroupeFileProxy* UBCGroupeFileProxy::CheckFilenameExists(UBrainCloudWrapper* in_braincloudWrapper,
	FString in_groupId, FString in_folderPath, FString in_filename)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->CheckFilenameExists(in_groupId, in_folderPath, in_filename, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::CheckFullpathFilenameExists(UBrainCloudWrapper* in_braincloudWrapper,
	FString in_groupId, FString in_fullPathFilename)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->CheckFullpathFilenameExists(in_groupId, in_fullPathFilename, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::CopyFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename,
	bool in_overwriteIfPresent)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->CopyFile(in_groupId, in_fileId, in_version, in_newTreeId, in_treeVersion, in_newFilename, in_overwriteIfPresent, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::DeleteFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId, int in_version, FString in_filename)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->DeleteFile(in_groupId, in_fileId, in_version, in_filename, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::GetCDNUrl(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->GetCDNUrl(in_groupId, in_fileId, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::GetFileInfo(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->GetFileInfo(in_groupId, in_fileId, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::GetFileInfoSimple(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_folderPath, FString in_filename)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->GetFileInfoSimple(in_groupId, in_folderPath, in_filename, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::GetFileList(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_folderPath, bool in_recurse)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->GetFileList(in_groupId, in_folderPath, in_recurse, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::MoveFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename,
	bool in_overwriteIfPresent)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->MoveFile(in_groupId, in_fileId, in_version, in_newTreeId, in_treeVersion, in_newFilename, in_overwriteIfPresent, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::MoveUserToGroupFile(UBrainCloudWrapper* in_braincloudWrapper,
	FString in_userCloudPath, FString in_userCloudFilename, FString in_groupId, FString in_groupTreeId,
	FString in_groupFilename, UBrainCloudGroupACL* in_groupFileAclJson, bool in_overwriteIfPresent)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->MoveUserToGroupFile(in_userCloudPath, in_userCloudFilename, in_groupId, in_groupTreeId, in_groupFilename, in_groupFileAclJson, in_overwriteIfPresent, Proxy);
	return Proxy;
}

UBCGroupeFileProxy* UBCGroupeFileProxy::UpdateFileInfo(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId,
	FString in_fileId, int in_version, FString in_newFilename, UBrainCloudGroupACL* in_newAclJson)
{
	UBCGroupeFileProxy* Proxy = NewObject<UBCGroupeFileProxy>();
	UBCWrapperProxy::GetBrainCloudInstance(in_braincloudWrapper)->getGroupFileService()->UpdateFileInfo(in_groupId, in_fileId, in_version, in_newFilename, in_newAclJson, Proxy);
	return Proxy;
}





