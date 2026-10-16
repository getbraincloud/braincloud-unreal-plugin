// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Signs a request payload for an app.
 *
 * @param Payload The request payload
 * @return The signature to send with the request
 */
typedef TFunction<FString(const TArray<uint8>& Payload)> FBCAppProfile;
