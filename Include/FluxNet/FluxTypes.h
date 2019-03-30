#pragma once
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#endif

using uint8             = unsigned char;
using uint16            = unsigned short;
using uint32            = unsigned int;
using uint64            = unsigned long long;

using int8              = char;
using int16             = short;
using int32             = int;
using int64             = long long;

using Bool              = uint8;
using Float             = float;

#define True        1
#define False       0

using PointerType       = unsigned long long;

#ifdef _WIN32
using PlatformSocket        = SOCKET;
using PlatformSocketAddr    = SOCKADDR_IN;
#endif

#define MAX_PACKET_SEQUENCE     32
#define FLUX_RESEND_DELAY_MS    100 

namespace Flux
{
    enum class ESocketFamily
    {
        IPV4,
#ifdef FLUX_ENABLE_IPV6
        IPV6
#endif
    };

    struct SocketDescriptor
    {
        ESocketFamily           Family;
        PlatformSocket          Socket;
    };

    struct SocketAddressDescriptor
    {
        union Address
        {
            uint32              IPV4Address;
#ifdef FLUX_ENABLE_IPV6
            uint16              IPV6Address[8];
#endif // FLUX_ENABLE_IPV6

        };

        Bool Compare(SocketAddressDescriptor const& descriptor) const
        {
            if (descriptor.Port == Port && descriptor.Address.sin_addr.s_addr == descriptor.Address.sin_addr.s_addr)
            {
                return True;
            }

            return False;
        }

        uint16                  Port;
        PlatformSocketAddr      Address;
    };

    enum class EConnectionState
    {
        Idle,
        HandshakeStarted,
        HandshakeInProgress,
        Connected,
        Disconnected
    };
}