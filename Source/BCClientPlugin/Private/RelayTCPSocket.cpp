
#include "RelayTCPSocket.h"

#if PLATFORM_WINDOWS
#include "Runtime/Core/Public/Windows/AllowWindowsPlatformTypes.h"
#include "windows.h" // any native windows header
#include <winsock2.h>
#include "Runtime/Core/Public/Windows/HideWindowsPlatformTypes.h"
#endif

#include "SocketSubsystem.h"
#include <Interfaces/IPv4/IPv4Address.h>
#include <Common/TcpSocketBuilder.h>
#include <ConvertUtilities.h>


BrainCloud::RelayTCPSocket::RelayTCPSocket(const FString& host, int port)
{
	if (!m_connectedSocket) {

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

		m_remoteAddr = SocketSubsystem->CreateInternetAddr();

		m_remoteAddr->SetIp(*host, m_isValid);
		m_remoteAddr->SetPort(port);

		if (!m_isValid) {
			UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Socket IP address was not valid! %s"), *host);
			return;
		}

		int32 SendSize = 1024;

		m_connectedSocket = FTcpSocketBuilder("TCP Socket")
			.AsReusable();

		m_isConnected = m_connectedSocket->Connect(*m_remoteAddr);

		UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayTCPSocket Socket initialized"));
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

	//Handle receiving packets
	uint32 BufferSize = 4096;
	m_receiveBuffer.Empty();
	m_receiveBuffer.SetNumZeroed(BufferSize);

	int32 BytesRead = 0;
	bool bReceived = m_connectedSocket->Recv(m_receiveBuffer.GetData(), m_receiveBuffer.Num(), BytesRead);
	m_receiveBuffer.SetNumZeroed(BytesRead);


	if (bReceived && BytesRead > 0) {
		m_packetQueue.Add(m_receiveBuffer);
	}

	//Handle sending packets
	if (m_sendPacketQueue.Num() == 0) return;

	int32 BytesSent = 0;
	bool bSendSuccessfull = m_connectedSocket->Send(m_sendPacketQueue[0].GetData(), m_sendPacketQueue[0].Num(), BytesSent);
	m_sendPacketQueue.RemoveAt(0);
}

void BrainCloud::RelayTCPSocket::updateConnection()
{
	//Nothing necessary to implement here
}

void BrainCloud::RelayTCPSocket::send(const uint8* pData, int size)
{
	m_sendPacket.Empty();
	m_sendPacket.Append(pData, size);

	m_sendPacketQueue.Add(MoveTemp(m_sendPacket));
}

const uint8* BrainCloud::RelayTCPSocket::peek(int& size)
{
	FScopeLock Lock(&m_mutex);
	if (m_packetQueue.Num() == 0) return nullptr;

	m_currentPacket = m_packetQueue[0];
	m_packetQueue.RemoveAt(0);

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

