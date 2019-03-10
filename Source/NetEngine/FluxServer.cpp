#include "NetEngine/FluxServer.h"
#include "Network/FluxNetModulePlatform.h"

Flux::Server::Server(ServerConfig const& config)
{
    m_config = config;
}

Flux::Server::~Server()
{
}

Bool Flux::Server::Listen()
{
    m_socket = NetModule::Instance()->CreateSocket(m_config.Family);

    SocketAddressDescriptor addressDescriptor;
    addressDescriptor.Family = m_config.Family;
    addressDescriptor.Port = m_config.HostPort;

    if (NetModule::Instance()->CreateNetworkAddress(addressDescriptor, m_config.HostIP.c_str()) == False)
    {
        return False;
    }

    if (NetModule::Instance()->SetNonBlocking(m_socket, True) == False)
    {
        return False;
    }

    if (NetModule::Instance()->BindSocket(m_socket, addressDescriptor) == False)
    {
        return False;
    }

    return True;
}

void Flux::Server::Update()
{
    int32 recvSize = NetModule::Instance()->RecvMessage(m_socket, m_recvAddress, m_recvBuffer, FLUX_NET_MTU);
    if (recvSize > 0)
    {
        printf("Message : %s", m_recvBuffer);
    }
}
