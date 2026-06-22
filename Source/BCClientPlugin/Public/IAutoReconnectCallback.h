// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BCCLIENTPLUGIN_API IAutoReconnectCallback
{
public:
	virtual ~IAutoReconnectCallback() {}

	virtual void autoReconnectSuccess(const FString& jsonData) = 0;
	virtual void autoReconnectFailed(const FString& jsonData) = 0;
};
