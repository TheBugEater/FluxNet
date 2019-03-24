#include "NetEngine/FluxClient.h"
#include "NetEngine/FluxPeer.h"
#include "Network/FluxNetModulePlatform.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{
    Client::Client(ClientConfig const& config)
    {
        m_config = config;
    }

    Client::~Client()
    {
    }

    Bool Client::Connect()
    {
        m_socket = NetModule::CreateSocket(m_config.Family);

        SocketAddressDescriptor addressDescriptor;
        addressDescriptor.Port = m_config.ServerPort;

        if (NetModule::CreateNetworkAddress(m_socket, addressDescriptor, m_config.ServerIP.c_str()) == False)
        {
            return False;
        }

        m_peer = FluxNew Peer(addressDescriptor);
        
        // Connection Message
        HelloMessage message;
        message.m_magicNumber = 0xDF3B2ECF;
        m_peer->Send(&message);
        m_peer->Send(&message);
        
        return True;
    }

    void Client::Update()
    {
        FlushSend();
    }

    void Client::FlushSend()
    {
        uint8 buffer[FLUX_NET_MTU];
        BinaryStream binaryStream;

        m_peer->SerializePacket(&binaryStream);
        if (!binaryStream.IsEmpty())
        {
            uint32 length = binaryStream.GetBuffer(buffer, FLUX_NET_MTU);
            NetModule::SendMessage(m_socket, m_peer->GetAddressDescriptor(), buffer, length);
        }
    }
}
