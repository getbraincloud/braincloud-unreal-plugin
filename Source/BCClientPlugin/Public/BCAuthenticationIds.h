#pragma once
#include "BCAuthenticationIds.generated.h"

USTRUCT(BlueprintType, Category = "BrainCloud")
struct FAuthenticationIds
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, Category = "BrainCloud")
	FString externalId = TEXT("");

	UPROPERTY(BlueprintReadWrite, Category = "BrainCloud")
	FString authenticationToken = TEXT("");

	UPROPERTY(BlueprintReadWrite, Category = "BrainCloud")
	FString authenticationSubType = TEXT("");

	FAuthenticationIds() {}

	FAuthenticationIds(FString in_externalId, FString in_authenticationToken, FString in_authenticationSubType)
		: externalId(in_externalId), authenticationToken(in_authenticationToken), authenticationSubType(in_authenticationSubType)
	{
	}
};
