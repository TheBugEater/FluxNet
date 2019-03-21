#pragma once
#include "Serializer/FluxReflection.h"

namespace Flux
{
    class MessageHeader : public ISerializable
    {
        FLUX_CLASS(MessageHeader)

    private:
        uint16          m_sequence;
        uint8           m_channel;
    };
}
