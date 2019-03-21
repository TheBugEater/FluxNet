#pragma once
#include "Serializer/FluxReflection.h"

namespace Flux
{
    class MessageHeader : public ISerializable
    {
        FLUX_CLASS(MessageHeader)

    public:
        uint16          m_sequence;
        uint8           m_channel;
    };

    class PacketHeader : public ISerializable
    {
        FLUX_CLASS(PacketHeader)

    public:
        uint32          m_ackBits;
        uint16          m_currentAck;
    };
}
