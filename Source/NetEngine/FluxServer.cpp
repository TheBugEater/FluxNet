#include "NetEngine/FluxServer.h"
#include "NetEngine/FluxPeer.h"
#include "Network/FluxNetModulePlatform.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "Utils/FluxNetAllocator.h"
#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    Server::Server(ServerConfig const& config)
    {
        m_config = config;

        memset(m_peers, 0, sizeof(m_peers));

        m_testPacket = new TestMessage();
        m_testPacket->m_value = 103434;
    }

    Server::~Server()
    {
    }

    Bool Server::Listen()
    {
        m_socket = NetModule::CreateSocket(m_config.Family);

        SocketAddressDescriptor addressDescriptor;
        addressDescriptor.Port = m_config.HostPort;

        if (NetModule::CreateNetworkAddress(m_socket, addressDescriptor, m_config.HostIP.c_str()) == False)
        {
            return False;
        }

        if (NetModule::SetNonBlocking(m_socket, True) == False)
        {
            return False;
        }

        if (NetModule::BindSocket(m_socket, addressDescriptor) == False)
        {
            return False;
        }

        return True;
    }

    void Server::Update()
    {
        int32 recvSize = NetModule::RecvMessage(m_socket, m_recvAddress, m_recvBuffer, FLUX_NET_MTU);
        if (recvSize > 0)
        {
            Peer* peer = FindPeerByAddress(m_recvAddress);
            if (!peer)
            {
                m_peers[0] = FluxNew Peer(m_recvAddress);
                peer = m_peers[0];
            }

            // Test Packet
            m_peers[0]->Send(m_testPacket);
        }
        FlushSend();
    }

    void Server::FlushSend()
    {
        uint8 buffer[FLUX_NET_MTU];
        BinaryStream binarStream;

        for (uint32 i = 0; i < FLUX_MAX_PEERS; i++)
        {
            if (m_peers[i] == nullptr)
            {
                continue;
            }

            m_peers[i]->SerializePacket(&binarStream);

            if (!binarStream.IsEmpty())
            {
                uint32 length = binarStream.GetBuffer(buffer, FLUX_NET_MTU);
                NetModule::SendMessage(m_socket, m_peers[i]->GetAddressDescriptor(), buffer, length);
            }
        }
    }

    Peer* Server::FindPeerByAddress(SocketAddressDescriptor const& descriptor)
    {
        for (uint32 i = 0; i < FLUX_MAX_PEERS; i++)
        {
            if (m_peers[i] && m_peers[i]->GetAddressDescriptor().Compare(descriptor))
            {
                return m_peers[i];
            }
        }

        return nullptr;
    }

}
