#include "NetEngine/FluxChannel.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "NetEngine/FluxNetNotificationHandler.h"
#include "NetEngine/FluxNetEngineDefines.h"

namespace Flux
{

    Channel::Channel(Peer* pOwner, EChannelType type)
        : m_pOwner(pOwner)
        , m_channelType(type)
        , m_sendSequence(0)
        , m_recvSequence(0)
        , m_pBinaryStream(nullptr)
    {
        m_pBinaryStream = FluxNew BinaryStream();
    }

    Channel::~Channel()
    {
        if (m_pBinaryStream)
        {
            FluxDelete m_pBinaryStream;
            m_pBinaryStream = nullptr;
        }
    }

    Bool Channel::Send(ISerializable* object)
    {
        m_pBinaryStream->Reset();

        object->Serialize(m_pBinaryStream);
        if (m_pBinaryStream->IsEmpty())
        {
            return False;
        }

        if (m_sendSequence > 256)
        {
            return False;
        }

        Message* pMessage = FluxNew Message();
        pMessage->m_channel = 0;
        pMessage->m_sequence = m_sendSequence++;
        pMessage->m_stream.LoadFromBinaryStream(m_pBinaryStream);

        m_sentMessages.InsertAt(pMessage->m_sequence, pMessage);
        m_outgoingQueue.Push(pMessage);

        return True;
    }

    Message* Channel::PopOutgoingMessage()
    {
        if (m_outgoingQueue.IsEmpty())
        {
            return nullptr;
        }

        Message* pMessage = m_outgoingQueue.Front();
        m_outgoingQueue.Pop();

        return pMessage;
    }

    void Channel::PushIncomingMessage(Message* pMessage)
    {
        m_incomingQueue.InsertAt(pMessage->m_sequence, pMessage);
    }

    void Channel::ProcessNotifications(INetNotificationHandler* pHandler)
    {
        while (m_incomingQueue.IsFrontExist())
        {
            auto pMessage = m_incomingQueue.Front();
            if (pMessage)
            {
                pMessage->m_stream.LoadToBinaryStream(m_pBinaryStream);
                ISerializable* pUserMessage = ClassFactory::Instance()->GenerateClassHierachy(m_pBinaryStream);
                pHandler->OnMessage(m_pOwner, pUserMessage);
                FluxDelete pMessage;

                m_pBinaryStream->Reset();
            }

            m_incomingQueue.Pop();
        }
    }

    void Channel::Update(std::chrono::system_clock::time_point const& currentTime)
    {
        for (uint32 index = 0; index < 1024; index++)
        {
            if (!m_sentMessages.IsPresent(index))
            {
                continue;
            }
            auto pMessage = m_sentMessages.FindAt(index);

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - pMessage->m_lastSentTime).count();
            if (elapsed > FLUX_RESEND_DELAY_MS)
            {
                pMessage->m_lastSentTime = currentTime;
                m_outgoingQueue.Push(pMessage);
            }
        }
    }

    void Channel::RemoveAckedSequence(uint16 sequence)
    {
        if (m_sentMessages.IsPresent(sequence))
        {
            auto pMessage = m_sentMessages.FindAt(sequence);
            if (pMessage)
            {
                m_sentMessages.RemoveAt(sequence);
                // Release pMessage
            }
        }
    }
}