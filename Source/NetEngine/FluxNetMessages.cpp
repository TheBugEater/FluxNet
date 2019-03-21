#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    FLUX_BEGIN_CLASS_ROOT(MessageHeader)
        FLUX_PROPERTY(m_sequence)
        FLUX_PROPERTY(m_channel)
    FLUX_END_CLASS(MessageHeader)

    FLUX_BEGIN_CLASS_ROOT(PacketHeader)
        FLUX_PROPERTY(m_ackBits)
        FLUX_PROPERTY(m_currentAck)
    FLUX_END_CLASS(PacketHeader)
}