#include "NetEngine/FluxChannel.h"
#include "Utils/FluxAllocator.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{

    Channel::Channel(EChannelType type)
        : m_channelType(type)
        , m_sendSequence(0)
        , m_recvSequence(0)
    {
    }

    void Channel::Send(ISerializable* object)
    {
        BinaryStream stream;
        object->Serialize(&stream);
        if (stream.IsEmpty())
        {
            return;
        }

        Message* pMessage = FluxNew Message();
        pMessage->m_channel = 0;
        pMessage->m_sequence = m_sendSequence;
        pMessage->m_stream.LoadFromBinaryStream(&stream);
        //pMessage->m_serializable = object->Clone();
        //pMessage->m_bufferLength = stream.GetBuffer(pMessage->m_buffer, stream.GetSize());

        m_outgoingQueue.push(pMessage);
    }

    Message* Channel::PopSendQueue()
    {
        if (m_outgoingQueue.empty())
        {
            return nullptr;
        }

        Message* pMessage = m_outgoingQueue.front();
        m_outgoingQueue.pop();
        return pMessage;
    }

}