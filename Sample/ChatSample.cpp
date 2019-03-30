#include "NetEngine/FluxNetEngine.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "Serializer/FluxReflection.h"
#include "Utils/FluxHash.h"
#include "Serializer/Reflection/FluxClassFactory.h"
#include "NetEngine/FluxNetNotificationHandler.h"
#include "NetEngine/FluxNetMessages.h"
#include "NetEngine/FluxPeer.h"
#include "Utils/FluxQueue.h"

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

class Testo2 : public Flux::ISerializable
{
public:
    FLUX_CLASS(Testo2)

    uint32  test2Value1;
    uint32  test2Value2;
    Testo   testo;
};

FLUX_BEGIN_CLASS_ROOT(Testo)
    FLUX_PROPERTY(value)
    FLUX_PROPERTY(valueShort)
FLUX_END_CLASS(Testo)

FLUX_BEGIN_CLASS_ROOT(Testo2)
    FLUX_PROPERTY(test2Value1)
    FLUX_PROPERTY(test2Value2)
    FLUX_PROPERTY(testo)
FLUX_END_CLASS(Testo2)

class Notifications : public Flux::INetNotificationHandler
{
public:
    virtual void OnMessage(Peer* pPeer, ISerializable* message) override
    {
        if (message->GetClass()->GetClassID() == Flux::HelloMessage::ClassID)
        {
            Testo test;
            test.value = 1000;
            test.valueShort = 2434;
            pPeer->Send(&test);
        }
        else if (message->GetClass()->GetClassID() == Testo::ClassID)
        {
            Flux::HelloMessage hello;
            hello.m_magicNumber = 0xFFFFAABB;
            pPeer->Send(&hello);
        }
    }
};

void TestCircularQueue()
{
    CircularSequenceBuffer<uint32, 1024> sequenceBuffer;

    // Pop empty
    sequenceBuffer.Pop();
    
    for (uint16 i = 0; i < 1025; i++)
    {
        sequenceBuffer.Push(rand() % 256);
    }
}

int main()
{
    // Test CRC
    static_assert(Flux::CRC32("Hello World") == 0x4a17b156, "CRC Failed");

    TestCircularQueue();

    Notifications notifier;

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
    pServer->SetNotificationHandler(&notifier);

    // Loopback Client
    ClientConfig clientConfig;
    clientConfig.Family = Flux::ESocketFamily::IPV4;
    clientConfig.ServerIP = "127.0.0.1";
    clientConfig.ServerPort = HOST_PORT;
    Client* pClient = NetEngine::Instance()->CreateClient(clientConfig);
    pClient->Connect();
    pClient->SetNotificationHandler(&notifier);

    Testo2 test, test2;
    test.testo.value = 2323;
    test.testo.valueShort = 97;
    test.test2Value1 = 645;
    test.test2Value2 = 353;
    Flux::BinaryStream stream;
    test.Serialize(&stream);

    stream.Reset();
    ISerializable* value = ClassFactory::Instance()->GenerateClassHierachy(&stream);

    while (True)
    {
        NetEngine::Instance()->Update();
        Sleep(10);
    }

    NetEngine::DestroyInstance();
    return 0;
}