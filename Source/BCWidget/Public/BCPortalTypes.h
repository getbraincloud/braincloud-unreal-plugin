// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCPortalTypes.generated.h"

/**
 * A team of the logged in brainCloud portal account.
 */
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

/**
 * An app of a brainCloud portal team.
 */
USTRUCT(BlueprintType)
struct FBCPortalApp
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppId;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppName;

    /**
     * The id of this app's parent app. Empty if it has no parent.
     */
    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString ParentAppId;
};

/**
 * A template app that new apps can be created from.
 */
USTRUCT(BlueprintType)
struct FBCPortalTemplateApp
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppId;

    UPROPERTY(BlueprintReadOnly, Category = "BrainCloud Portal")
    FString AppName;
};
