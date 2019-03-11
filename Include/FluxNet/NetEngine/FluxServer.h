#pragma once
#include "NetEngine/FluxNetEngineDefines.h"
#include "Utils/FluxNetUpdater.h"
#include "FluxNetDefines.h"
#include <string>

namespace Flux
{
    class Peer;

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

        virtual void            Update() override;

    public:
        Bool                    Listen();

    private:
        Peer*                   m_peers[FLUX_MAX_PEERS];
        SocketDescriptor        m_socket;

        uint8                   m_recvBuffer[FLUX_NET_MTU];
        SocketAddressDescriptor m_recvAddress;

        ServerConfig            m_config;

        friend class            NetEngine;
    };
}