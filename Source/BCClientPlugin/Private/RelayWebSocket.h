#pragma once

#include "IRelaySocket.h"
#include "Runtime/Launch/Resources/Version.h"

#if PLATFORM_UWP
    #if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 24
        #if PLATFORM_HTML5
        #endif
    #endif
#else
#if PLATFORM_WINDOWS
#   include "Windows/WindowsHWrapper.h"
#    include "Windows/AllowWindowsPlatformTypes.h"
#endif

#define UI UI_ST
#undef UI

#if PLATFORM_WINDOWS
#    include "Windows/HideWindowsPlatformTypes.h"
#endif
#endif

#include "WinWebSocketBase.h"

namespace BrainCloud
{
    class RelayWebSocket : public IRelaySocket, public IWinWebSocketBaseCallbacks
    {
    public:
        RelayWebSocket(const FString &host, int port, bool sslEnabled);
        virtual ~RelayWebSocket();

        bool isConnected() override;
        bool isValid() override;
        
        void update() override;
        void updateConnection() override; // Call each frame when !isConnected and isValid

        void send(const uint8* pData, int size) override;
        const uint8* peek(int& size) override; // Returns null if nothing to read

        void close() override;

    private:
        void OnConnectError(const FString& error) override;
        void OnClosed() override;
        void OnConnectComplete() override;
        void OnReceiveData(const TArray<uint8>& data) override;

        FCriticalSection m_mutex;
        bool m_isConnected = false;
        bool m_isSocketConnected = false;
        bool m_isValid = true;

	    UWinWebSocketBase *m_connectedSocket = nullptr;
        TArray<TArray<uint8>> m_packetQueue;
        TArray<TArray<uint8>> m_sendPacketQueue;
        TArray<uint8> m_currentPacket;
        TArray<uint8> m_sendPacket;
    };
};
