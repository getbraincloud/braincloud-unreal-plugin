
#include "RelayUDPSocket.h"

#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include "windows.h" // any native windows header
#include "SocketSubsystem.h"
#include <Interfaces/IPv4/IPv4Address.h>
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"
#include <Common/UdpSocketBuilder.h>

BrainCloud::RelayUDPSocket::RelayUDPSocket(const FString& host, int port)
{
	
	if (!m_connectedSocket) {
		
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Initializing"));

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		FIPv4Address Addr;
		FIPv4Address::Parse(TEXT("127.0.0.1"), Addr);

		

		m_remoteAddr = SocketSubsystem->CreateInternetAddr();
		m_localAddr = SocketSubsystem->CreateInternetAddr();

		m_remoteAddr->SetIp(*host, m_isValid);
		m_remoteAddr->SetPort(port);

		m_localAddr->SetIp(Addr.Value);
		m_localAddr->SetPort(54000);

		FIPv4Endpoint localEndPoint = FIPv4Endpoint(FIPv4Address::Any, 54000);

		if (!m_isValid) {
			UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Socket IP address was not valid! %s"), *host);
			return;
		}

		int32 SendSize = 2 * 1024 * 1024;

		//m_connectedSocket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("UDP Socket"), false);
		m_connectedSocket = FUdpSocketBuilder("UDP Socket")
			.AsNonBlocking()
			.AsReusable()
			.BoundToEndpoint(localEndPoint)
			.WithReceiveBufferSize(SendSize)
			.WithSendBufferSize(SendSize)
			.WithBroadcast();


		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Socket initialized"));
		
	}
	else {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Socket already initialized"));
	}
	
}

BrainCloud::RelayUDPSocket::~RelayUDPSocket()
{
	close();
}

bool BrainCloud::RelayUDPSocket::isConnected()
{
	return m_connectedSocket->GetConnectionState() == SCS_Connected;
	//return false;
}

bool BrainCloud::RelayUDPSocket::isValid()
{
	return m_isValid;
}

void BrainCloud::RelayUDPSocket::update()
{
	/*
	TArray<uint8> RecvData;
	RecvData.Init(0, 65507);

	uint32 Size;
	while (m_connectedSocket->HasPendingData(Size)) {
		int32 Read = 0;
		m_connectedSocket->RecvFrom(RecvData.GetData(), Size, Read, *m_localAddr);

		if (Read > 0) {
			FString ReceivedString = BytesToString(RecvData.GetData(), Read);
			UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket ReceivedData %s"), *ReceivedString );
		}
	}
	*/
}

void BrainCloud::RelayUDPSocket::updateConnection()
{
}

void BrainCloud::RelayUDPSocket::send(const uint8* pData, int size)
{
	if (!m_connectedSocket || !isConnected()) {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Can't send message because socket not connected"));
		return;
	}

	FString messageData = BytesToString(pData, size);

	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Sending message %s of size %z"), *messageData, size);

	int32 BytesSent = 0;

	TArray<uint8> data;
	data.SetNum(size, false);
	memcpy(data.GetData(), pData, size);
	m_connectedSocket->SendTo(data.GetData(), size, BytesSent, *m_remoteAddr);

	if (BytesSent <= 0) {
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket %s"), *Str);
		return;
	}

	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Message Send Successfull, Bytes Sent = %s"), BytesSent);
}
/*
void BrainCloud::RelayUDPSocket::recv(const FArrayReaderPtr& ArrayReaderPtr, const FIPv4Endpoint& EndPt)
{
}
*/
const uint8* BrainCloud::RelayUDPSocket::peek(int& size)
{
	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Peeking"));

	TArray<uint8> RecvData;
	RecvData.Init(0, 65507);

	uint32 Size;
	while (m_connectedSocket->HasPendingData(Size)) {
		int32 Read = 0;
		m_connectedSocket->RecvFrom(RecvData.GetData(), Size, Read, *m_remoteAddr);

		if (Read > 0) {
			FString ReceivedString = BytesToString(RecvData.GetData(), Read);
			UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket ReceivedData %s"), *ReceivedString);
			size = Size;
			return RecvData.GetData();
		}
	}

	return nullptr;
}

void BrainCloud::RelayUDPSocket::close()
{
	
	if (m_connectedSocket) {
		m_connectedSocket->Close();
		delete m_connectedSocket;
	}
	
}

