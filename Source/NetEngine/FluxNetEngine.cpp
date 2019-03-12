#include "NetEngine/FluxNetEngine.h"
#include "NetEngine/FluxServer.h"
#include "Utils/FluxNetAllocator.h"
#include "Network/FluxNetModulePlatform.h"

namespace Flux
{
    IMPLEMENT_SINGLETON(NetEngine)

    NetEngine::NetEngine()
    {
        NetModule::Initialize();
    }

    NetEngine::~NetEngine()
    {
        NetModule::Shutdown();
    }

    Flux::Server* NetEngine::CreateServer(ServerConfig const& serverConfig)
    {
        auto pServer = FluxNew Server(serverConfig);
        m_updaters.push_back(pServer);
        return pServer;
    }

    void NetEngine::CloseServer()
    {
    }

    Flux::Client* NetEngine::CreateClient(ClientConfig const& clientConfig)
    {
        auto pClient = FluxNew Client(clientConfig);
        m_updaters.push_back(pClient);
        return pClient;
    }

    void NetEngine::CloseClient()
    {
    }

    void NetEngine::Update()
    {
        for (auto pUpdater : m_updaters)
        {
            pUpdater->Update();
        }
    }
}