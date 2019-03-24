#pragma once
#include "FluxTypes.h"
#include <queue>

namespace Flux
{
    class ISerializable;
    class Message;

    enum class EChannelType
    {
        Reliable_Ordered,
        Unreliable_Unordered
    };

    class Channel
    {
    public:
        Channel(EChannelType type);

        void                    Send(ISerializable* object);

        Message*                PopSendQueue();

    private:
        EChannelType            m_channelType;

        uint16                  m_sendSequence;
        uint16                  m_recvSequence;

        std::queue<Message*>    m_outgoingQueue;
        std::queue<Message*>    m_sentQueue;
    };
}
