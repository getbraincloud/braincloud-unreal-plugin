// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BCCLIENTPLUGIN_API IReconnectCallback
{
public:
	virtual ~IReconnectCallback() {}

	virtual void reconnectSuccess(const FString& jsonData) = 0;
	virtual void reconnectFailed(const FString& jsonData) = 0;
};
