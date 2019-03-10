#include "NetEngine/FluxNetEngine.h"

#define HOST_IP     "0.0.0.0"
#define HOST_PORT   3850

using namespace Flux;

int main()
{
    ServerConfig config;
    config.Family = Flux::ESocketFamily::IPV4;
    config.HostIP = HOST_IP;
    config.HostPort = HOST_PORT;

    NetEngine::CreateInstance();
    Server* pServer = NetEngine::Instance()->CreateServer(config);
    if (pServer->Listen() == False)
    {
        return -1;
    }

    while (True)
    {
        NetEngine::Instance()->Update();
    }

    NetEngine::DestroyInstance();
    return 0;
}