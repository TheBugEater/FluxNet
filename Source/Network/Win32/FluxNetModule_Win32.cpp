#include "Network/Win32/FluxNetModule_Win32.h"
#include <WS2tcpip.h>

namespace Flux
{
    Bool    NetModuleWindows::Initialize()
    {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            return False;
        }

        return True;
    }

    SocketDescriptor  NetModuleWindows::CreateSocket(ESocketFamily family)
    {
        int ipFamily = (family == ESocketFamily::IPV4) ? AF_INET : AF_INET6;

        SocketDescriptor sock;
        sock.Socket = socket(ipFamily, SOCK_DGRAM, IPPROTO_UDP);
        sock.Family = family;

        return sock;
    }

    Bool   NetModuleWindows::BindSocket(SocketDescriptor const& sock, SocketAddressDescriptor const& address)
    {
        if (bind(sock.Socket, (const sockaddr*)&address.Address, sizeof(address.Address)) == SOCKET_ERROR)
        {
            return False;
        }
        return True;
    }

    int32   NetModuleWindows::SendMessage(SocketDescriptor const& sock, SocketAddressDescriptor const& address, const uint8* message, const int32 messageLen)
    {
        int32 sentSize = sendto(sock.Socket, (const char*)message, messageLen, 0, (sockaddr*)&address.Address, sizeof(address.Address));
        return sentSize;
    }

    int32   NetModuleWindows::RecvMessage(SocketDescriptor const& sock, SocketAddressDescriptor& address, uint8* message, const int32 messageLen)
    {
        int32 fromLength = sizeof(address.Address);
        int32 recvSize = recvfrom(sock.Socket, (char*)message, messageLen, 0, (sockaddr*)&address.Address, &fromLength);
        return recvSize;
    }

    void    NetModuleWindows::CloseSocket(SocketDescriptor const& socket)
    {
        closesocket(socket.Socket);
    }

    void    NetModuleWindows::Shutdown()
    {
    }

    Bool    NetModuleWindows::SetNonBlocking(SocketDescriptor const& sock, Bool enable)
    {
        u_long enableNonBlocking = enable;
        int32 result = ioctlsocket(sock.Socket, FIONBIO, &enableNonBlocking);
        if (result != SOCKET_ERROR)
        {
            return True;
        }

        return False;
    }

    uint16 NetModuleWindows::HostToNetworkShort(uint16 value)
    {
        return htons(value);
    }

    uint32 NetModuleWindows::HostToNetworkLong(uint32 value)
    {
        return htonl(value);
    }

    uint16 NetModuleWindows::NetworkToHostShort(uint16 value)
    {
        return ntohs(value);
    }

    uint32 NetModuleWindows::NetworkToHostLong(uint32 value)
    {
        return ntohl(value);
    }

    Bool NetModuleWindows::CreateNetworkAddress(SocketDescriptor const& sock, SocketAddressDescriptor& descriptor, const char* src)
    {
        descriptor.Address.sin_family = (sock.Family == ESocketFamily::IPV4) ? AF_INET : AF_INET6;
        descriptor.Address.sin_port = HostToNetworkShort(descriptor.Port);

        if (InetPton(descriptor.Address.sin_family, src, &descriptor.Address.sin_addr) != 1)
        {
            return False;
        }

        return True;
    }
}