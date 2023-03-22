
#include "RelayUDPSocket.h"

#if PLATFORM_WINDOWS
#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include "windows.h" // any native windows header
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

#include "SocketSubsystem.h"
#include <Interfaces/IPv4/IPv4Address.h>
#include <Common/UdpSocketBuilder.h>


BrainCloud::RelayUDPSocket::RelayUDPSocket(const FString& host, int port)
{
	if (!m_connectedSocket) {
		
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

		m_connectedSocket = FUdpSocketBuilder("UDP Socket")
			.AsNonBlocking()
			.AsReusable()
			.WithReceiveBufferSize(SendSize)
			.WithSendBufferSize(SendSize)
			.WithBroadcast();


		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Socket initialized [!]"));
	}
	else {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Socket already initialized"));
	}
	
}

BrainCloud::RelayUDPSocket::~RelayUDPSocket()
{
	FScopeLock Lock(&m_mutex);
	close();
}

bool BrainCloud::RelayUDPSocket::isConnected()
{
	if (!m_connectedSocket) {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket socket is null"));
		return false;
	}

	return m_connectedSocket->GetConnectionState() == SCS_Connected;
}

bool BrainCloud::RelayUDPSocket::isValid()
{
	return m_isValid;
}

void BrainCloud::RelayUDPSocket::update()
{
	if (!m_connectedSocket || !isConnected()) {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Can't update because socket not connected"));
		return;
	}

	TArray<uint8> RecvData;
	RecvData.Init(0, 57344);

	uint32 BufferSize;
	while (m_connectedSocket->HasPendingData(BufferSize)) {
		int32 Read = 0;
		m_connectedSocket->RecvFrom(RecvData.GetData(), BufferSize, Read, *m_remoteAddr);

		if (Read > 0) {
			FString ReceivedString = BytesToString(RecvData.GetData(), Read);
			//UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayUDPSocket Received Data: %s"), *ReceivedString);
			if (!m_connectedSocket) return;
			m_packetQueue.Add(RecvData);
		}
	}
	
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

	FScopeLock Lock(&m_mutex);
	FString messageData = BytesToString(pData, size);

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
}

const uint8* BrainCloud::RelayUDPSocket::peek(int& size)
{
	
	FScopeLock Lock(&m_mutex);
	if (m_packetQueue.Num() == 0) return nullptr;

	m_currentPacket = m_packetQueue[0];
	m_packetQueue.RemoveAt(0);
	
	auto packetSize = (int)ntohs(*(u_short*)m_currentPacket.GetData());
	size = packetSize;
	return m_currentPacket.GetData();
}

void BrainCloud::RelayUDPSocket::close()
{
	FScopeLock Lock(&m_mutex);
	if (m_connectedSocket != nullptr) {

		m_connectedSocket->Close();
		delete m_connectedSocket;
		m_connectedSocket = nullptr;
	}
	m_isConnected = false;
	m_isValid = false;
	m_packetQueue.Empty(0);
}

