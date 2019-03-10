#pragma once
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

#define True        1
#define False       0

using PointerType       = unsigned long long;

#ifdef _WIN32
using PlatformSocket        = SOCKET;
using PlatformSocketAddr    = SOCKADDR_IN;
#endif