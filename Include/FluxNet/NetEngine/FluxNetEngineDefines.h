#pragma once
#include "FluxTypes.h"

#define FLUX_MAX_PEERS      256
#define FLUX_MAX_CHANNELS   64
#define FLUX_NET_MTU        1400

enum class EPeerState
{
    None,
    Connecting,
    Connected,
    Disconnecting,
    Disconnected
};

namespace Flux
{
}