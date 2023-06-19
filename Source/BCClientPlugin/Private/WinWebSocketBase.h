#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/WebSockets/Public/IWebSocket.h"
#include "Runtime/Online/WebSockets/Public/IWebSocketsManager.h"
#include "Runtime/Online/WebSockets/Public/WebSocketsModule.h"
#include "WinWebSocketBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(WinWebSocket, Log, All);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinWebSocketClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinWebSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketReceiveData, const TArray<uint8>&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketReceiveMessage, const FString&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinWebSocketConnectError, const FString&, error);

class IWinWebSocketBaseCallbacks
{
public:
	virtual void OnConnectError(const FString& error) = 0;
	virtual void OnClosed() = 0;
	virtual void OnConnectComplete() = 0;
	virtual void OnReceiveData(const TArray<uint8>& data) = 0;
};

UCLASS(Blueprintable, BlueprintType)
class BCCLIENTPLUGIN_API UWinWebSocketBase : public UObject
{
	GENERATED_BODY()
public:
	UWinWebSocketBase();

	void SetupSocket(const FString& url);

	void Connect();

	void Close();

	void SendText(const FString& data);

	void SendData(const TArray<uint8>& data);

	bool IsConnected();

	FWinWebSocketReceiveData OnReceiveData;

	FWinWebSocketReceiveMessage OnReceiveMessage;

	FWinWebSocketConnected OnConnectComplete;

	FWinWebSocketClosed OnClosed;

	FWinWebSocketConnectError OnConnectError;

	IWinWebSocketBaseCallbacks* mCallbacks;

private:
	FString BytesToString(const void* Data, SIZE_T Size);

	TSharedPtr<IWebSocket> WebSocket;
	FString ServerUrl;
	TArray<FString> mSendQueue;
	TArray<TArray<uint8>> mSendQueueData;
	
};

