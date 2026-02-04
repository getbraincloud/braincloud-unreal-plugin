// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BCCLIENTPLUGIN_API ILongSessionCallback
{
public:
	virtual ~ILongSessionCallback() {}

	virtual void longSessionSuccess(const FString& jsonData) = 0;
	virtual void longSessionFailed(const FString& jsonData) = 0;
};
