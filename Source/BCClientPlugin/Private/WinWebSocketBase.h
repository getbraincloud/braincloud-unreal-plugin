#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/WebSockets/Public/IWebSocket.h"
#include "Runtime/Online/WebSockets/Public/IWebSocketsManager.h"
#include "Runtime/Online/WebSockets/Public/WebSocketsModule.h"
#include "WinWebSocketBase.generated.h"

class BrainCloudClient;
DEFINE_LOG_CATEGORY_STATIC(WinWebSocket, Log, All);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinWebSocketClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinWebSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketReceiveData, const TArray<uint8>&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketReceiveMessage, const FString&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketConnectError, const FString&, error);

class IWinWebSocketBaseCallbacks
{
public:
	virtual ~IWinWebSocketBaseCallbacks() {}
	virtual void OnConnectError(const FString& error) = 0;
	virtual void OnClosed() = 0;
	virtual void OnConnectComplete() = 0;
	virtual void OnReceiveData(const TArray<uint8>& data) = 0;
};

UCLASS(Blueprintable, BlueprintType)
class BCCLIENTPLUGIN_API UWinWebSocketBase : public UObject
{
	GENERATED_BODY()

protected:
	void BeginDestroy() override;

public:
	UWinWebSocketBase();

	void SetupSocket(const FString& url, BrainCloudClient* in_client);

	void Connect();

	void Close();

	void SendText(const FString& data);

	void SendData(const TArray<uint8>& data);

	void ResetCallbacks();

	bool IsConnected();

	bool IsLoggingEnabled();

	FWinWebSocketReceiveData OnReceiveData;

	FWinWebSocketReceiveMessage OnReceiveMessage;

	FWinWebSocketConnected OnConnectComplete;

	FWinWebSocketClosed OnClosed;

	FWinWebSocketConnectError OnConnectError;

	IWinWebSocketBaseCallbacks* mCallbacks = nullptr;

private:
	FString BytesToString(const void* Data, SIZE_T Size);
	BrainCloudClient *mClient = nullptr;
	TSharedPtr<IWebSocket> WebSocket;
	FString ServerUrl;
	TArray<FString> mSendQueue;
	TArray<TArray<uint8>> mSendQueueData;
	bool mIsLoggingEnabled;
	
};

