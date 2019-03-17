#include "NetEngine/FluxNetEngine.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "Serializer/FluxSerializer.h"
#include "Serializer/FluxSerializableClass.h"
#include "Serializer/FluxSerializable.h"

#define HOST_IP     "0.0.0.0"
#define HOST_PORT   3850

using namespace Flux;

class Testo : public Flux::ISerializable
{
public:
    FLUX_CLASS(Testo)

    uint32  value;
    uint16  valueShort;
};

class Testo2 : public Testo
{
public:
    FLUX_CLASS(Testo2)

    uint32  test2Value1;
    uint32  test2Value2;
};

FLUX_BEGIN_CLASS_ROOT(Testo)
FLUX_PROPERTY(&Testo::value)
FLUX_PROPERTY(&Testo::valueShort)
FLUX_CLASS_END(Testo)

FLUX_BEGIN_CLASS(Testo2, Testo)
FLUX_PROPERTY(&Testo2::test2Value1)
FLUX_PROPERTY(&Testo2::test2Value2)
FLUX_CLASS_END(Testo2)



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

    Testo2 test, test2;
    test.value = 2323;
    test.valueShort = 97;
    test.test2Value1 = 645;
    test.test2Value2 = 353;
    Flux::BinaryStream stream;
    test.Serialize(&stream);

    stream.Reset();
    test2.Deserialize(&stream);


    while (True)
    {
        NetEngine::Instance()->Update();
        Sleep(10);
    }

    NetEngine::DestroyInstance();
    return 0;
}