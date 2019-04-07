#include "NetEngine/FluxNetMessages.h"

namespace Flux
{
    FLUX_BEGIN_CLASS_ROOT(Message)
        FLUX_PROPERTY(m_sequence)
        FLUX_PROPERTY(m_channel)
        FLUX_PROPERTY(m_stream)
    FLUX_END_CLASS(Message)

    FLUX_BEGIN_CLASS_ROOT(Packet)
        FLUX_PROPERTY(m_ackBits)
        FLUX_PROPERTY(m_lastReceivedSequence)
        FLUX_PROPERTY(m_packetSequence)
        FLUX_PROPERTY(m_stream)
    FLUX_END_CLASS(Packet)

    FLUX_BEGIN_CLASS_ROOT(HelloMessage)
        FLUX_PROPERTY(m_magicNumber)
    FLUX_END_CLASS(HelloMessage)

    FLUX_BEGIN_CLASS_ROOT(PingMessage)
    FLUX_END_CLASS(PingMessage)
}