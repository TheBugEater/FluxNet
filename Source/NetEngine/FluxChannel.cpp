#include "NetEngine/FluxChannel.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"
#include "NetEngine/FluxNetNotificationHandler.h"

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

    void Channel::Send(ISerializable* object)
    {
        m_pBinaryStream->Reset();

        object->Serialize(m_pBinaryStream);
        if (m_pBinaryStream->IsEmpty())
        {
            return;
        }

        Message* pMessage = FluxNew Message();
        pMessage->m_channel = 0;
        pMessage->m_sequence = m_sendSequence++;
        pMessage->m_stream.LoadFromBinaryStream(m_pBinaryStream);

        m_outgoingQueue.push(pMessage);
    }

    Message* Channel::PopOutgoingMessage()
    {
        if (m_outgoingQueue.empty())
        {
            return nullptr;
        }

        Message* pMessage = m_outgoingQueue.front();
        m_outgoingQueue.pop();

        m_sentQueue.push(pMessage);

        return pMessage;
    }

    void Channel::PushIncomingMessage(Message* pMessage)
    {
        m_incomingQueue.push(pMessage);
    }

    void Channel::ProcessNotifications(INetNotificationHandler* pHandler)
    {
        while (!m_incomingQueue.empty())
        {
            auto pMessage = m_incomingQueue.front();
            if (pMessage)
            {
                pMessage->m_stream.LoadToBinaryStream(m_pBinaryStream);
                ISerializable* pUserMessage = ClassFactory::Instance()->GenerateClassHierachy(m_pBinaryStream);
                pHandler->OnMessage(m_pOwner, pUserMessage);
                FluxDelete pMessage;
            }

            m_incomingQueue.pop();
        }
    }
}