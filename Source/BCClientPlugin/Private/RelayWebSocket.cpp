#include "RelayWebSocket.h"
#include "WinWebSocketBase.h"

#define MAX_PAYLOAD 1024


namespace BrainCloud
{
    RelayWebSocket::RelayWebSocket(const FString &host, int port, bool sslEnabled)
    {
        FString url = (sslEnabled ? "wss://" : "ws://") + host + ":" + FString::FromInt(port);

        // lazy load
        if (m_connectedSocket == nullptr)
        {
            m_connectedSocket = NewObject<UWinWebSocketBase>();
            m_connectedSocket->SetupSocket(url);

            m_connectedSocket->mCallbacks = this;

            m_connectedSocket->AddToRoot();
        }

        m_connectedSocket->Connect();
    }

    RelayWebSocket::~RelayWebSocket()
    {
        close();

        FScopeLock Lock(&m_mutex);
        if (m_connectedSocket != nullptr)
        {
            m_connectedSocket->RemoveFromRoot();
            m_connectedSocket = nullptr;
        }
    }

    bool RelayWebSocket::isConnected()
    {
        return m_connectedSocket->IsConnected();
    }
    
    bool RelayWebSocket::isValid()
    {
        return m_isValid;
    }

    void RelayWebSocket::updateConnection()
    {
        FScopeLock Lock(&m_mutex);
        m_isConnected = isConnected();
    }

    void RelayWebSocket::send(const uint8* pData, int size)
    {
        UE_LOG(WinWebSocket, Log, TEXT("[RelayWebSocket - Send()]"));

        TArray<uint8> data;
        data.SetNum(size, false);
        memcpy(data.GetData(), pData, size);
        m_sendPacketQueue.Add(data);

    }

    const uint8* RelayWebSocket::peek(int& size)
    {
        //UE_LOG(WinWebSocket, Log, TEXT("[RelayWebSocket - Peek()] packet queue size: %d"), m_packetQueue.Num());

        {
            FScopeLock Lock(&m_mutex);
            if (m_packetQueue.Num() == 0) return nullptr;

            m_currentPacket = m_packetQueue[0];
            m_packetQueue.RemoveAt(0);
        }

        FString packetString = BytesToString(m_currentPacket.GetData(), m_currentPacket.Num());

        UE_LOG(WinWebSocket, Log, TEXT("Peek: packet: %s"), *packetString);

        size = (int)m_currentPacket.Num();
        return m_currentPacket.GetData();
    }

    void RelayWebSocket::close()
    {
        FScopeLock Lock(&m_mutex);
        m_isConnected = false;
        m_isSocketConnected = false;
        m_isValid = false;
        m_packetQueue.Empty(0);
        m_connectedSocket->Close();
    }

    void RelayWebSocket::OnConnectError(const FString& error)
    {
        UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayWebSocket OnConnectError: %s"), *error);
        close();

        FScopeLock Lock(&m_mutex);
        if (!m_connectedSocket) return;
        m_isValid = false;
    }

    void RelayWebSocket::OnClosed()
    {
        UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayWebSocket OnClosed"));
        close();
    }

    void RelayWebSocket::OnConnectComplete()
    {
        UE_LOG(LogBrainCloudRelayComms, Log, TEXT("RelayWebSocket OnConnectComplete"));
        FScopeLock Lock(&m_mutex);
        if (!m_connectedSocket) return;
        m_isSocketConnected = true;
    }

    void RelayWebSocket::OnReceiveData(const TArray<uint8>& data)
    {
        UE_LOG(LogBrainCloudRelayComms, Warning, TEXT("[RelayWebSocket - OnReceiveData] Size: %d"), data.Num());
        FScopeLock Lock(&m_mutex);
        if (!m_connectedSocket) return;
        m_packetQueue.Add(data);
    }

    void RelayWebSocket::update()
    {
        if (m_connectedSocket->IsConnected()) {
            if (m_sendPacketQueue.Num() == 0) return;

            TArray<uint8> currentPacket = m_sendPacketQueue[0];
            m_connectedSocket->SendData(currentPacket);
            m_sendPacketQueue.RemoveAt(0);
        }
    }
};
