#include "NetEngine/FluxNetEngine.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "Serializer/FluxSerializer.h"

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

    // Loopback Client
    ClientConfig clientConfig;
    clientConfig.Family = Flux::ESocketFamily::IPV4;
    clientConfig.ServerIP = "127.0.0.1";
    clientConfig.ServerPort = 3850;
    Client* pClient = NetEngine::Instance()->CreateClient(clientConfig);
    pClient->Connect();

    Test test;
    test.number = 121223;
    test.value = 5343;
    Flux::BinaryStream stream;
    test.Serialize(&stream);

    stream.Reset();

    Test test1;
    test1.Deserialize(&stream);

    while (True)
    {
        NetEngine::Instance()->Update();
        Sleep(10);
    }

    NetEngine::DestroyInstance();
    return 0;
}