#pragma once
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#include <stdio.h>
#endif

// #undef DEBUG_RELIABLE_LAYER

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

static void SocketLog(const char * pszFormat, ...)
{
    char szBuf[MAX_PATH];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuf, MAX_PATH, MAX_PATH, pszFormat, ap);
    va_end(ap);

    WCHAR wszBuf[MAX_PATH] = { 0 };
    MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");

    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
    printf("%s\n", szBuf);
}

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

    static uint32      GetRoundIndex(uint32 max, uint32 index)
    {
        return (max + index) % max;
    }
}