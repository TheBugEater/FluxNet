#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    FLUX_BEGIN_CLASS_ROOT(Message)
        FLUX_PROPERTY(m_sequence)
        FLUX_PROPERTY(m_channel)
        FLUX_PROPERTY(m_bufferLength)
    FLUX_END_CLASS(Message)

    FLUX_BEGIN_CLASS_ROOT(PacketHeader)
        FLUX_PROPERTY(m_ackBits)
        FLUX_PROPERTY(m_currentAck)
    FLUX_END_CLASS(PacketHeader)

    FLUX_BEGIN_CLASS_ROOT(ConnectMessage)
        FLUX_PROPERTY(m_magicNumber)
    FLUX_END_CLASS(ConnectMessage)

    FLUX_BEGIN_CLASS_ROOT(TestMessage)
        FLUX_PROPERTY(m_value)
    FLUX_END_CLASS(TestMessage)
}