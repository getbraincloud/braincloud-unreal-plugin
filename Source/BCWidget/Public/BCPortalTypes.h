// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCPortalTypes.generated.h"

USTRUCT(BlueprintType)
struct FBCPortalTeam
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString TeamId;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString TeamName;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    bool bApiEnabled = false;
};

USTRUCT(BlueprintType)
struct FBCPortalApp
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppId;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppName;
};

USTRUCT(BlueprintType)
struct FBCPortalTemplateApp
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppId;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppName;
};
