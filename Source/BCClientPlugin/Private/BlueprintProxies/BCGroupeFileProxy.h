#pragma once
#include "CoreMinimal.h"
#include "BCBlueprintCallProxyBase.h"
#include "BrainCloudGroupACL.h"
#include "BCGroupeFileProxy.generated.h"

class UBrainCloudWrapper;
UCLASS()
class UBCGroupeFileProxy : public UBCBlueprintCallProxyBase
{
	GENERATED_BODY()
	
public:
	UBCGroupeFileProxy(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* CheckFilenameExists(UBrainCloudWrapper* in_braincloudWrapper,FString in_groupId, FString in_folderPath, FString in_filename);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* CheckFullpathFilenameExists(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fullPathFilename);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* CopyFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* DeleteFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId, int in_version, FString in_filename);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* GetCDNUrl(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* GetFileInfo(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* GetFileInfoSimple(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_folderPath,FString in_filename);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* GetFileList(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_folderPath, bool in_recurse);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* MoveFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId, int in_version, FString in_newTreeId, int in_treeVersion, FString in_newFilename, bool in_overwriteIfPresent);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* MoveUserToGroupFile(UBrainCloudWrapper* in_braincloudWrapper, FString in_userCloudPath, FString in_userCloudFilename, FString in_groupId, FString in_groupTreeId, FString in_groupFilename, UBrainCloudGroupACL* in_groupFileAclJson, bool in_overwriteIfPresent);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "BrainCloud|GroupFile")
	static UBCGroupeFileProxy* UpdateFileInfo(UBrainCloudWrapper* in_braincloudWrapper, FString in_groupId, FString in_fileId, int in_version, FString in_newFilename, UBrainCloudGroupACL* in_newAclJson);
	
};
