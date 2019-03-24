#pragma once
#include "FluxTypes.h"
#include <queue>

namespace Flux
{
    class ISerializable;
    class Peer;
    class INetNotificationHandler;
    class BinaryStream;
    class Message;

    enum class EChannelType
    {
        Reliable_Ordered,
        Unreliable_Unordered
    };

    class Channel
    {
    public:
        Channel(Peer* pOwner, EChannelType type);
        virtual ~Channel();

        void                            Send(ISerializable* object);

        Message*                        PopOutgoingMessage();
        void                            PushIncomingMessage(Message* pMessage);

        void                            ProcessNotifications(INetNotificationHandler* pHandler);
    private:
        EChannelType                    m_channelType;
        Peer*                           m_pOwner;

        uint16                          m_sendSequence;
        uint16                          m_recvSequence;

        std::queue<Message*>            m_incomingQueue;

        BinaryStream*                   m_pBinaryStream;
        std::queue<Message*>            m_outgoingQueue;
        std::queue<Message*>            m_sentQueue;
    };
}
