// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once
#include "BrainCloudAppDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FBrainCloudAppDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString ServerUrl = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString AppId = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString AppSecret = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString Version = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString S2SKey = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "brainCloud Init Params")
		FString S2SUrl = "";
};
