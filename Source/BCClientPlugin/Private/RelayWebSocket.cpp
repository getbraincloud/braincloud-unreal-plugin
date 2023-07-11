#include "RelayWebSocket.h"
#include "WinWebSocketBase.h"
#include <ConvertUtilities.h>

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
        m_sendPacket.Empty();
        m_sendPacket.Append(pData, size);
        m_sendPacketQueue.Add(MoveTemp(m_sendPacket));
    }

    const uint8* RelayWebSocket::peek(int& size)
    {
        {
            FScopeLock Lock(&m_mutex);
            if (m_packetQueue.Num() == 0) return nullptr;

            m_currentPacket = m_packetQueue[0];
            m_packetQueue.RemoveAt(0);
        }

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
        FScopeLock Lock(&m_mutex);
        if (!m_connectedSocket) return;
        m_packetQueue.Add(data);
    }

    void RelayWebSocket::update()
    {
        double now = FPlatformTime::Seconds();

        if (m_connectedSocket->IsConnected()) {
            if (m_sendPacketQueue.Num() == 0) return;

            m_connectedSocket->SendData(m_sendPacketQueue[0]);
            m_sendPacketQueue.RemoveAt(0);
        }
    }
};
