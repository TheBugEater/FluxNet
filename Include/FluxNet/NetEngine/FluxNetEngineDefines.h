#pragma once
#include "FluxTypes.h"

#define FLUX_MAX_PEERS          256
#define FLUX_MAX_CHANNELS       64
#define FLUX_NET_MTU            1400
#define FLUX_MAX_RECV_PACKETS   1024
#define FLUX_MAX_SEND_PACKETS   128

enum class EPeerState
{
    None,
    Connecting,
    Connected,
    Disconnecting,
    Disconnected
};
