#include "NetEngine/FluxClient.h"
#include "NetEngine/FluxPeer.h"
#include "Network/FluxNetModulePlatform.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{
    Client::Client(ClientConfig const& config)
        : m_config(config)
        , m_pNotifier(nullptr)
        , m_peer(nullptr)
    {
    }

    Client::~Client()
    {
    }

    Bool Client::Connect()
    {
        Reset();

        m_socket = NetModule::CreateSocket(m_config.Family);

        SocketAddressDescriptor addressDescriptor;
        addressDescriptor.Port = m_config.ServerPort;

        if (NetModule::CreateNetworkAddress(m_socket, addressDescriptor, m_config.ServerIP.c_str()) == False)
        {
            return False;
        }

        if (NetModule::SetNonBlocking(m_socket, True) == False)
        {
            return False;
        }

        m_peer = FluxNew Peer(addressDescriptor);
        
        // Connection Message
        HelloMessage message;
        message.m_magicNumber = 0xDF3B2ECF;
        m_peer->Send(&message);
        
        return True;
    }

    void Client::Reset()
    {
        if (m_peer)
        {
            FluxDelete m_peer;
        }
    }

    void Client::SetNotificationHandler(INetNotificationHandler* pHandler)
    {
        m_pNotifier = pHandler;
    }

    void Client::Update()
    {
        int32 recvSize = NetModule::RecvMessage(m_socket, m_recvAddress, m_recvBuffer, FLUX_NET_MTU);
        if (recvSize > 0)
        {
            BinaryStream stream;
            stream.LoadFromBuffer((uint8*)m_recvBuffer, recvSize);
            m_peer->ProcessIncomingPacket(&stream);
        }

        FlushSend();
        if (m_pNotifier)
        {
            m_peer->ProcessNotifications(m_pNotifier);
        }
    }

    void Client::FlushSend()
    {
        uint8 buffer[FLUX_NET_MTU];
        BinaryStream binaryStream;

        m_peer->CreateOutgoingPacket(&binaryStream);
        if (!binaryStream.IsEmpty())
        {
            uint32 length = binaryStream.GetBuffer(buffer, FLUX_NET_MTU);
            NetModule::SendMessage(m_socket, m_peer->GetAddressDescriptor(), buffer, length);
        }
    }
}
