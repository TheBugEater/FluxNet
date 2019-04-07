#pragma once
#include "FluxTypes.h"

#define FLUX_MAX_PEERS              256
#define FLUX_MAX_CHANNELS           64
#define FLUX_NET_MTU                1400
#define FLUX_MAX_RECV_PACKETS       1024
#define FLUX_MAX_SEND_PACKETS       1024
#define FLUX_MAX_MSGS_PER_CHANNEL   8
#define FLUX_PING_INTERVAL          5000
#define FLUX_RESEND_DELAY_MS        100 

enum class EPeerState
{
    None,
    Connecting,
    Connected,
    Disconnecting,
    Disconnected
};
