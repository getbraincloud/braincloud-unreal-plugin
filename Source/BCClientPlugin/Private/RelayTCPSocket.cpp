
#include "RelayTCPSocket.h"

#if PLATFORM_WINDOWS
#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include "windows.h" // any native windows header
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"
#include <WinSock2.h>
#endif

#include "SocketSubsystem.h"
#include <Interfaces/IPv4/IPv4Address.h>
#include <Common/TcpSocketBuilder.h>
#include <ConvertUtilities.h>


BrainCloud::RelayTCPSocket::RelayTCPSocket(const FString& host, int port)
{
	if (!m_connectedSocket) {

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		FIPv4Address Addr;
		FIPv4Address::Parse(TEXT("127.0.0.1"), Addr);

		m_remoteAddr = SocketSubsystem->CreateInternetAddr();
		m_localAddr = SocketSubsystem->CreateInternetAddr();

		m_remoteAddr->SetIp(*host, m_isValid);
		m_remoteAddr->SetPort(port);

		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Set remote host to %s:%d"), *host, port);

		m_localAddr->SetIp(Addr.Value);
		m_localAddr->SetPort(54000);

		if (!m_isValid) {
			UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Socket IP address was not valid! %s"), *host);
			return;
		}

		int32 SendSize = 1024;

		m_connectedSocket = FTcpSocketBuilder("TCP Socket")
			.AsReusable();
		//.BoundToEndpoint(m_remoteAddr)
		//.BoundToAddress(Addr)
		//.BoundToPort(54000);

		m_isConnected = m_connectedSocket->Connect(*m_remoteAddr);

		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Socket initialized [!] - connected? %s"), m_isConnected ? TEXT("True") : TEXT("False"));
	}
	else {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Socket already initialized"));
	}

}

BrainCloud::RelayTCPSocket::~RelayTCPSocket()
{
	FScopeLock Lock(&m_mutex);
	close();
}

bool BrainCloud::RelayTCPSocket::isConnected()
{
	if (!m_connectedSocket) {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket socket is null"));
		return false;
	}

	//UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket connection state: %d"), m_connectedSocket->GetConnectionState());

	return m_connectedSocket->GetConnectionState() == SCS_Connected;
}

bool BrainCloud::RelayTCPSocket::isValid()
{
	return m_isValid;
}

void BrainCloud::RelayTCPSocket::update()
{
	if (!m_connectedSocket || !isConnected()) {
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Can't update because socket not connected"));
		return;
	}

	uint32 BufferSize = 4096;
	TArray<uint8> receivedPacket;
	receivedPacket.SetNumZeroed(BufferSize);

	int32 BytesRead = 0;
	bool bReceived = m_connectedSocket->Recv(receivedPacket.GetData(), receivedPacket.Num(), BytesRead);


	receivedPacket.SetNumZeroed(BytesRead);

	if (bReceived && BytesRead > 0) {
		FString ReceivedData = ConvertUtilities::BCBytesToString(receivedPacket.GetData(), BytesRead);
		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Received %d bytes | data: %s"), BytesRead, *ReceivedData);
		m_packetQueue.Add(receivedPacket);
	}

	if (m_sendPacketQueue.Num() == 0) return;

	int32 BytesSent = 0;
	bool bSendSuccessfull = m_connectedSocket->Send(m_sendPacketQueue[0].GetData(), m_sendPacketQueue[0].Num(), BytesSent);
	m_sendPacketQueue.RemoveAt(0);

	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Send successfull? %s Bytes sent: %d"), bSendSuccessfull ? TEXT("True") : TEXT("False"), BytesSent);
}

void BrainCloud::RelayTCPSocket::updateConnection()
{

}

void BrainCloud::RelayTCPSocket::send(const uint8* pData, int size)
{
	m_sendPacket.Empty();
	m_sendPacket.Append(pData, size);

	FString messageData = ConvertUtilities::BCBytesToString(m_sendPacket.GetData(), m_sendPacket.Num());
	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Sending Data: %s"), *messageData);

	m_sendPacketQueue.Add(MoveTemp(m_sendPacket));
}

const uint8* BrainCloud::RelayTCPSocket::peek(int& size)
{

	FScopeLock Lock(&m_mutex);
	if (m_packetQueue.Num() == 0) return nullptr;

	m_currentPacket = m_packetQueue[0];
	m_packetQueue.RemoveAt(0);


	FString messageData = ConvertUtilities::BCBytesToString(m_currentPacket.GetData(), m_currentPacket.Num());
	UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Peek packet: %s"), *messageData);
	//auto packetSize = (int)ntohs(*(u_short*)m_currentPacket.GetData());
	size = m_currentPacket.Num();
	return m_currentPacket.GetData();
}

void BrainCloud::RelayTCPSocket::close()
{
	FScopeLock Lock(&m_mutex);
	if (m_connectedSocket != nullptr) {

		m_connectedSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_connectedSocket);
		m_connectedSocket = nullptr;
	}
	m_isConnected = false;
	m_isValid = false;
	m_packetQueue.Empty(0);
}

