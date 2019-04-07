#pragma once
#include "FluxTypes.h"
#include "Utils/FluxCircularBuffer.h"
#include <list>
#include <chrono>

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

        template<uint16 Capacity, Bool Overwrite = True>
        using MessageQueue = CircularSequenceBuffer<Message*, Capacity, Overwrite>;

        using SentMessageQueue = CircularSequenceBuffer<Message*, 1024, False>;

        Bool                            Send(ISerializable* object);

        Message*                        PopOutgoingMessage();
        void                            PushIncomingMessage(Message* pMessage);

        void                            ProcessNotifications(INetNotificationHandler* pHandler);
        void                            Update(std::chrono::system_clock::time_point const& currentTime);

        void                            RemoveAckedSequence(uint16 sequence);
    private:
        EChannelType                    m_channelType;
        Peer*                           m_pOwner;

        uint16                          m_sendSequence;
        uint16                          m_recvSequence;

        BinaryStream*                   m_pBinaryStream;

        MessageQueue<256, False>        m_incomingQueue;
        MessageQueue<256, False>        m_outgoingQueue;
        SentMessageQueue                m_sentMessages;
    };
}
