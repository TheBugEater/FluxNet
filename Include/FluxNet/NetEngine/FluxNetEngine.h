#pragma once
#include "FluxTypes.h"
#include "NetEngine/FluxServer.h"
#include "NetEngine/FluxClient.h"
#include <vector>

namespace Flux
{
    class NetEngine
    {
        DEFINE_SINGLETON(NetEngine)

    public:
        Server*                     CreateServer(ServerConfig const& serverConfig);
        void                        CloseServer();

        Client*                     CreateClient(ClientConfig const& clientConfig);
        void                        CloseClient();

        void                        Update();

    private:
        std::vector < NetUpdater*>  m_updaters;
    };
}