#include "WinWebSocketBase.h"
#include <ConvertUtilities.h>

UWinWebSocketBase::UWinWebSocketBase()
{
	FModuleManager::Get().LoadModuleChecked(TEXT("WebSockets"));
}

void UWinWebSocketBase::SetupSocket(const FString& url)
{
	if (url.IsEmpty()) {
		UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] URL is empty"));
		OnConnectError.Broadcast(TEXT("URL is empty"));
		return;
	}

	ServerUrl = url;
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(url);

	if (WebSocket.IsValid()) {
		WebSocket->OnMessage().AddLambda([this](const FString& data)
			{
				if(OnReceiveMessage.IsBound()) OnReceiveMessage.Broadcast(data);
			});

		WebSocket->OnRawMessage().AddLambda([this](const void* Data, SIZE_T Size, SIZE_T)
			{
				
				TArray<uint8> DataArray;
				DataArray.Append((const uint8*)Data, Size);

				if (mCallbacks) mCallbacks->OnReceiveData(DataArray);
				if (OnReceiveData.IsBound()) OnReceiveData.Broadcast(DataArray);
			});

		WebSocket->OnConnected().AddLambda([this]()
			{
				UE_LOG(WinWebSocket, Log, TEXT("[WinWebSocket] Connected"));
				if (mCallbacks) mCallbacks->OnConnectComplete();
				if (OnConnectComplete.IsBound()) OnConnectComplete.Broadcast();
			});

		WebSocket->OnClosed().AddLambda([this](uint32 StatusCode, const FString& Reason, bool bWasClean)
			{
				UE_LOG(WinWebSocket, Log, TEXT("[WinWebSocket] Closed - StatusCode: %d Reason: %s WasClean: %s"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
				if (mCallbacks) mCallbacks->OnClosed();
				if (OnClosed.IsBound()) OnClosed.Broadcast();
			});

		WebSocket->OnConnectionError().AddLambda([this](const FString& reason)
			{
				UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] Connection error: %s"), *reason);
				if (mCallbacks) mCallbacks->OnConnectError(reason);
				if (OnConnectError.IsBound()) OnConnectError.Broadcast(reason);
			});

	}
	else {
		UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] Couldn't setup"));
		OnConnectError.Broadcast(TEXT("WebSocket couldn't setup"));
	}


}

void UWinWebSocketBase::Connect()
{
	if (WebSocket.IsValid() && !WebSocket->IsConnected())
	{
		WebSocket->Connect();
		UE_LOG(LogTemp, Log, TEXT("[WebSocket] Connecting..."));
	}
}

void UWinWebSocketBase::Close()
{
	if (WebSocket.IsValid())
	{
		WebSocket->Close();
	}
}

void UWinWebSocketBase::SendText(const FString& data)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(data);
	}
}

void UWinWebSocketBase::SendData(const TArray<uint8>& data)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		SIZE_T DataSize = data.Num();
		const void* DataPtr = data.GetData();
		WebSocket->Send(DataPtr, DataSize, true);
	}
}

bool UWinWebSocketBase::IsConnected()
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

FString UWinWebSocketBase::BytesToString(const void* Data, SIZE_T Size)
{
	const uint8* ByteData = static_cast<const uint8*>(Data);
	FString message;

	for (SIZE_T i = 0; i < Size; i++)
	{
		message += FString::Printf(TEXT("%02X"), ByteData[i]);
	}

	return message;
}
