#include "NetEngine/FluxClient.h"
#include "Network/FluxNetModulePlatform.h"

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

        /*uint8   buffer[FLUX_NET_MTU];
        PacketHeader header;
        header.ChannelId = 0;
        header.SequenceId = 1;
        memcpy(buffer, &header, sizeof(PacketHeader));
        memcpy(buffer + sizeof(PacketHeader), "Hello", 5);

        if (NetModule::SendMessage(m_socket, addressDescriptor, buffer, sizeof(PacketHeader) + 5) < 0)
        {
            return False;
        }*/

        return True;
    }

    void Client::Update()
    {
    }
}
