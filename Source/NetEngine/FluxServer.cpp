#include "NetEngine/FluxServer.h"
#include "NetEngine/FluxPeer.h"
#include "Network/FluxNetModulePlatform.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    Server::Server(ServerConfig const& config)
        : m_config(config)
        , m_pNotifier(nullptr)
    {
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
                peer = FluxNew Peer(m_recvAddress);
                m_peers.push_back(peer);
            }

            BinaryStream stream;
            stream.LoadFromBuffer((uint8*)m_recvBuffer, recvSize);
            peer->ProcessIncomingPacket(&stream);
        }
        FlushSend();
        ProcessNotifications();
    }

    void Server::UpdatePeers()
    {
        for (auto peer : m_peers)
        {
            peer->Update();
        }
    }

    void Server::FlushSend()
    {
        uint8 buffer[FLUX_NET_MTU];
        BinaryStream binaryStream;

        for (auto peer : m_peers)
        {
            peer->CreateOutgoingPacket(&binaryStream);

            if (!binaryStream.IsEmpty())
            {
                uint32 length = binaryStream.GetBuffer(buffer, FLUX_NET_MTU);
                NetModule::SendMessage(m_socket, peer->GetAddressDescriptor(), buffer, length);
            }
        }
    }

    void Server::ProcessNotifications()
    {
        if (!m_pNotifier)
        {
            return;
        }

        // Later replace with a Hash Map
        for (auto peer : m_peers)
        {
            peer->ProcessNotifications(m_pNotifier);
        }
    }

    Peer* Server::FindPeerByAddress(SocketAddressDescriptor const& descriptor)
    {
        // Later replace with a Hash Map
        for (auto peer : m_peers)
        {
            if (peer->GetAddressDescriptor().Compare(descriptor))
            {
                return peer;
            }
        }

        return nullptr;
    }

    void Server::SetNotificationHandler(INetNotificationHandler* pHandler)
    {
        m_pNotifier = pHandler;
    }
}
