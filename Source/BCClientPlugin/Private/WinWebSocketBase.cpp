#include "WinWebSocketBase.h"
#include <ConvertUtilities.h>

#include "BrainCloudClient.h"

void UWinWebSocketBase::BeginDestroy()
{
	if (WebSocket.IsValid()) {
		WebSocket->OnClosed().Clear();
		WebSocket->OnConnected().Clear();
		WebSocket->OnMessage().Clear();
		WebSocket->OnRawMessage().Clear();
		WebSocket->OnConnectionError().Clear();
		WebSocket->Close();
		WebSocket.Reset();
	}

	Super::BeginDestroy();
}

UWinWebSocketBase::UWinWebSocketBase()
{
	FModuleManager::Get().LoadModuleChecked(TEXT("WebSockets"));
}

void UWinWebSocketBase::SetupSocket(const FString& url, BrainCloudClient* client)
{

	mClient = client;
	mIsLoggingEnabled = mClient->isLoggingEnabled();
	

	if (url.IsEmpty()) {
		if(mIsLoggingEnabled)
		{
			UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] URL is empty"));
		}
		OnConnectError.Broadcast(TEXT("URL is empty"));
		return;
	}

	ServerUrl = url;
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(url);

	if (WebSocket.IsValid()) {
		TWeakObjectPtr<UWinWebSocketBase> WeakThis = this;

		WebSocket->OnMessage().AddLambda([WeakThis](const FString& data)
			{
				if(WeakThis->OnReceiveMessage.IsBound()) WeakThis->OnReceiveMessage.Broadcast(data);
			});

		WebSocket->OnRawMessage().AddLambda([WeakThis](const void* Data, SIZE_T Size, SIZE_T)
			{
				
				TArray<uint8> DataArray;
				DataArray.Append((const uint8*)Data, Size);

				if (WeakThis->mCallbacks) WeakThis->mCallbacks->OnReceiveData(DataArray);
				if (WeakThis->OnReceiveData.IsBound()) WeakThis->OnReceiveData.Broadcast(DataArray);
			});

		WebSocket->OnConnected().AddLambda([WeakThis]()
			{
				if (WeakThis->IsLoggingEnabled())
				{
					UE_LOG(WinWebSocket, Log, TEXT("[WinWebSocket] Connected"));
				}
				if (WeakThis->mCallbacks) WeakThis->mCallbacks->OnConnectComplete();
				if (WeakThis->OnConnectComplete.IsBound()) WeakThis->OnConnectComplete.Broadcast();
			});

		WebSocket->OnClosed().AddLambda([WeakThis](uint32 StatusCode, const FString& Reason, bool bWasClean)
			{
				if (WeakThis->IsLoggingEnabled())
				{
					UE_LOG(WinWebSocket, Log, TEXT("[WinWebSocket] Closed - StatusCode: %d Reason: %s WasClean: %s"), StatusCode, *Reason, bWasClean ? TEXT("true") : TEXT("false"));
				}
				if (WeakThis->mCallbacks) WeakThis->mCallbacks->OnClosed();
				if (WeakThis->OnClosed.IsBound()) WeakThis->OnClosed.Broadcast();
			});

		WebSocket->OnConnectionError().AddLambda([WeakThis](const FString& reason)
			{
				if(WeakThis->IsLoggingEnabled())
				{
					UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] Connection error: %s"), *reason);
				}
				if (WeakThis->mCallbacks) WeakThis->mCallbacks->OnConnectError(reason);
				if (WeakThis->OnConnectError.IsBound()) WeakThis->OnConnectError.Broadcast(reason);
			});

	}
	else {
		if (IsLoggingEnabled())
		{
			UE_LOG(WinWebSocket, Warning, TEXT("[WinWebSocket] Couldn't setup"));
		}
		OnConnectError.Broadcast(TEXT("WebSocket couldn't setup"));
	}


}

void UWinWebSocketBase::Connect()
{
	if (WebSocket.IsValid() && !WebSocket->IsConnected())
	{
		WebSocket->Connect();
		if(mIsLoggingEnabled)
		{
			UE_LOG(LogTemp, Log, TEXT("[WebSocket] Connecting..."));
		}
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

void UWinWebSocketBase::ResetCallbacks()
{
	if (mCallbacks) {
		delete mCallbacks;
		mCallbacks = nullptr;
	}

	OnClosed.Clear();
	OnReceiveMessage.Clear();
	OnReceiveData.Clear();
	OnConnectComplete.Clear();
	OnConnectError.Clear();
	OnConnectError.Clear();

}

bool UWinWebSocketBase::IsConnected()
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

bool UWinWebSocketBase::IsLoggingEnabled()
{
	if (mClient != nullptr) {
		return mClient->isLoggingEnabled();
	}

	return mIsLoggingEnabled;
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
