#pragma once
#include "NetEngine/FluxNetEngineDefines.h"
#include "Utils/FluxNetUpdater.h"
#include "FluxNetDefines.h"
#include <string>
#include <vector>

namespace Flux
{
    class Peer;
    class INetNotificationHandler;

    struct ServerConfig
    {
        ESocketFamily   Family;
        uint16          HostPort;
        std::string     HostIP;
    };

    class Server : public NetUpdater
    {
    private:
        Server(ServerConfig const& config);
        virtual ~Server();

        virtual void                Update() override;
        void                        FlushSend();
        void                        ProcessNotifications();
        Peer*                       FindPeerByAddress(SocketAddressDescriptor const& descriptor);

    public:
        Bool                        Listen();
        void                        SetNotificationHandler(INetNotificationHandler* pHandler);

    private:
        std::vector<Peer*>          m_peers;
        SocketDescriptor            m_socket;

        uint8                       m_recvBuffer[FLUX_NET_MTU];
        SocketAddressDescriptor     m_recvAddress;

        ServerConfig                m_config;
        INetNotificationHandler*    m_pNotifier;
#ifdef _DEBUG
        class TestMessage*          m_testPacket;
#endif

        friend class                NetEngine;
    };
}