#include "NetEngine/FluxPeer.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{

    Peer::Peer(SocketAddressDescriptor const& descriptor)
        : m_address(descriptor)
        , m_channel(this, EChannelType::Reliable_Ordered)
        , m_packetSequence(0)
        , m_lastReceivedSequence(0)
    {
        m_pBinaryStream = FluxNew BinaryStream;
    }

    Peer::~Peer()
    {
        if (m_pBinaryStream)
        {
            FluxDelete m_pBinaryStream;
            m_pBinaryStream = nullptr;
        }
    }

    Bool Peer::Send(ISerializable* object)
    {
        return m_channel.Send(object);
    }

    void Peer::CreateOutgoingPacket(IStream* stream)
    {
        Message* pMessage = m_channel.PopOutgoingMessage();
        if (!pMessage)
        {
            return;
        }

        SentPacket sentPacket;
        // Currently only one channel per Peer
        sentPacket.Count = 1;
        uint32  ChannelIndex = 0;
        sentPacket.Channels[ChannelIndex].Count = 0;
        sentPacket.Channels[ChannelIndex].Channel = 0;

        auto currentTime = std::chrono::system_clock::now();
        m_pBinaryStream->Reset();
        while (pMessage)
        {
            if (sentPacket.Channels[ChannelIndex].Count == FLUX_MAX_MSGS_PER_CHANNEL)
            {
                break;
            }

            sentPacket.Channels[ChannelIndex].Sequences[sentPacket.Channels[ChannelIndex].Count] = pMessage->m_sequence;
            sentPacket.Channels[ChannelIndex].Count++;

            pMessage->m_lastSentTime = currentTime;
            pMessage->Serialize(m_pBinaryStream);
            pMessage = m_channel.PopOutgoingMessage();
        }

        Packet currentPacket;
        currentPacket.m_packetSequence = GetNextSequence();
        currentPacket.m_lastReceivedSequence = m_lastReceivedSequence;
        currentPacket.m_ackBits = GetLastAckedPackets();
        currentPacket.m_stream.LoadFromBinaryStream(m_pBinaryStream);

        m_sentQueue.InsertAt(currentPacket.m_packetSequence, sentPacket);

        currentPacket.Serialize(stream);
    }

    void Peer::ProcessIncomingPacket(IStream* stream)
    {
        auto message = ClassFactory::Instance()->GenerateClassHierachyType<Packet>(stream);
        if (message)
        {
            m_recvQueue.InsertAt(message->m_packetSequence, True);
            m_lastReceivedSequence = message->m_packetSequence;
            ProcessReceivedAcks(message->m_lastReceivedSequence, message->m_ackBits);

            m_pBinaryStream->Reset();
            message->m_stream.LoadToBinaryStream(m_pBinaryStream);
            auto otherMessage = ClassFactory::Instance()->GenerateClassHierachyType<Message>(m_pBinaryStream);
            while (otherMessage)
            {
                // @todo: retrieve the channel then push -> otherMessage->m_channel;
                m_channel.PushIncomingMessage(otherMessage);
                otherMessage = ClassFactory::Instance()->GenerateClassHierachyType<Message>(m_pBinaryStream);
            }
        }
    }

    void Peer::ProcessNotifications(INetNotificationHandler* pHandler)
    {
        m_channel.ProcessNotifications(pHandler);
    }

    void Peer::Update()
    {
        auto currentTime = std::chrono::system_clock::now();
        m_channel.Update(currentTime);
    }

    SocketAddressDescriptor const& Peer::GetAddressDescriptor() const
    {
        return m_address;
    }

    void Peer::ProcessReceivedAcks(uint16 sequence, uint32 ackBits)
    {
        uint32 index = GetRoundIndex(FLUX_MAX_SEND_PACKETS, sequence - 32);
        for (uint32 bitIndex = 0; bitIndex <= 32; bitIndex++, index++)
        {
            if (m_sentQueue.IsPresent(index) == True)
            {
                auto const& sentPacket = m_sentQueue.FindAt(index);
                for (uint32 channelIndex = 0; channelIndex < sentPacket.Count; channelIndex++)
                {
                    for (uint32 msgIndex = 0; msgIndex < sentPacket.Channels[channelIndex].Count; msgIndex++)
                    {
                        m_channel.RemoveAckedSequence(sentPacket.Channels[channelIndex].Sequences[msgIndex]);
                    }
                }
            }
        }

    }

    uint32 Peer::GetLastAckedPackets() const
    {
        uint32 acks = 0;
        uint32 index = GetRoundIndex(FLUX_MAX_RECV_PACKETS, m_lastReceivedSequence - 32);
        for (uint32 bitIndex = 0; bitIndex < 32; bitIndex++, index++)
        {
            if (m_recvQueue.IsPresent(index) == True)
            {
                acks |= 1 << bitIndex;
            }
        }
        return acks;
    }

    uint32 Peer::GetNextSequence()
    {
        m_packetSequence = m_packetSequence++ % FLUX_MAX_SEND_PACKETS;
        return m_packetSequence;
    }
}