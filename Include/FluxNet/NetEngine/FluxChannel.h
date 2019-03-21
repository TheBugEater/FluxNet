#pragma once
#include "FluxTypes.h"

namespace Flux
{
    enum class EChannelType
    {
        Reliable_Ordered,
        Unreliable_Unordered
    };

    class Channel
    {
    public:
        Channel(EChannelType type)
            : m_channelType(type)
        {
        }

    private:
        EChannelType    m_channelType;

        uint16          m_sendSequence;
        uint16          m_recvSequence;
    };
}
