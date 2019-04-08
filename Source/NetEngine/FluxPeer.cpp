#include "NetEngine/FluxPeer.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"


namespace Flux
{

    Peer::Peer(SocketAddressDescriptor const& descriptor)
        : m_address(descriptor)
        , m_packetSequence(0)
        , m_lastReceivedSequence(0)
    {
        m_pBinaryStream = FluxNew BinaryStream;

        m_channels[ProtocolID] = new Channel(this, "Protocol", EChannelType::Reliable_Ordered);

        // Later Create a Config to have default channels
        constexpr uint32 DefaultID = CRC32("DefaultReliable");
        m_channels[DefaultID] = new Channel(this, "DefaultReliable", EChannelType::Reliable_Ordered);
    }

    Peer::~Peer()
    {
        if (m_pBinaryStream)
        {
            FluxDelete m_pBinaryStream;
            m_pBinaryStream = nullptr;
        }
    }

    Bool Peer::Send(const int8* pChannelName, ISerializable* object)
    {
        uint32 ChannelID = CRC32(pChannelName, (uint32)strlen(pChannelName));
        auto it = m_channels.find(ChannelID);
        if(it != m_channels.end())
        {
            auto pChannel = it->second;
            return pChannel->Send(object);
        }

        return False;
    }

    void Peer::CreateOutgoingPacket(IStream* stream)
    {
        SentPacket sentPacket;
        sentPacket.Count = 0;

        m_pBinaryStream->Reset();

        for (auto it = m_channels.begin(); it != m_channels.end(); it++)
        {
            auto pChannel = it->second;
            Message* pMessage = pChannel->PopOutgoingMessage();
            if (!pMessage)
            {
                continue;
            }
            
            sentPacket.Channels[sentPacket.Count].Count = 0;
            sentPacket.Channels[sentPacket.Count].Channel = pChannel->GetChannelID();

            auto currentTime = std::chrono::system_clock::now();
            while (pMessage)
            {
                if (sentPacket.Channels[sentPacket.Count].Count == FLUX_MAX_MSGS_PER_CHANNEL)
                {
                    break;
                }

                sentPacket.Channels[sentPacket.Count].Sequences[sentPacket.Channels[sentPacket.Count].Count] = pMessage->m_sequence;
                sentPacket.Channels[sentPacket.Count].Count++;

                pMessage->m_lastSentTime = currentTime;
                pMessage->Serialize(m_pBinaryStream);
                pMessage = pChannel->PopOutgoingMessage();
            }

            sentPacket.Count++;
        }
        if (m_pBinaryStream->GetSize() > 0)
        {
            Packet currentPacket;
            currentPacket.m_stream.LoadFromBinaryStream(m_pBinaryStream);

            currentPacket.m_packetSequence = GetNextSequence();
            currentPacket.m_lastReceivedSequence = m_lastReceivedSequence;
            currentPacket.m_ackBits = GetLastAckedPackets();

            m_sentQueue.InsertAt(currentPacket.m_packetSequence, sentPacket);
			
			// Serialize at last, Otherwise we might miss information written to the packet later.
            currentPacket.Serialize(stream);
#ifdef DEBUG_RELIABLE_LAYER
            SocketLog("[%x] Packet Sequence %d",this, currentPacket.m_packetSequence);
#endif
        }
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
                auto foundIt = m_channels.find(otherMessage->m_channel);
                if (foundIt != m_channels.end())
                {
                    auto pChannel = foundIt->second;
                    pChannel->PushIncomingMessage(otherMessage);
                    otherMessage = ClassFactory::Instance()->GenerateClassHierachyType<Message>(m_pBinaryStream);
                }
            }
        }
    }

    void Peer::ProcessNotifications(INetNotificationHandler* pHandler)
    {
        for (auto it = m_channels.begin(); it != m_channels.end(); it++)
        {
            auto pChannel = it->second;
            pChannel->ProcessNotifications(pHandler);
        }
    }

    void Peer::Update()
    {
        auto currentTime = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastPingTime).count();
        if (elapsed > FLUX_PING_INTERVAL)
        {
            PingMessage ping;
            Send("Protocol", &ping);
            m_lastPingTime = currentTime;
        }
        for (auto it = m_channels.begin(); it != m_channels.end(); it++)
        {
            auto pChannel = it->second;
            pChannel->Update(currentTime);
        }
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
            if (!(ackBits & (1 << bitIndex)))
            {
                continue;
            }
            if (m_sentQueue.IsPresent(index) == True)
            {
                auto const& sentPacket = m_sentQueue.FindAt(index);
                for (uint32 channelIndex = 0; channelIndex < sentPacket.Count; channelIndex++)
                {
                    auto foundIt = m_channels.find(sentPacket.Channels[channelIndex].Channel);
                    if (foundIt != m_channels.end())
                    {
                        auto pChannel = foundIt->second;
                        for (uint32 msgIndex = 0; msgIndex < sentPacket.Channels[channelIndex].Count; msgIndex++)
                        {
                            pChannel->RemoveAckedSequence(sentPacket.Channels[channelIndex].Sequences[msgIndex]);
                        }
                    }
                }
            }
        }

    }

    uint32 Peer::GetLastAckedPackets() const
    {
#ifdef DEBUG_RELIABLE_LAYER
//        SocketLog("Sending Ack for Packet");
#endif
        uint32 acks = 0;
        uint32 index = GetRoundIndex(FLUX_MAX_RECV_PACKETS, m_lastReceivedSequence - 32);
        for (uint32 bitIndex = 0; bitIndex < 32; bitIndex++, index++)
        {
            if (m_recvQueue.IsPresent(index) == True)
            {
#ifdef DEBUG_RELIABLE_LAYER
//              SocketLog("%d", GetRoundIndex(FLUX_MAX_RECV_PACKETS, index));
#endif
                acks |= 1 << bitIndex;
            }
        }
#ifdef DEBUG_RELIABLE_LAYER
//        SocketLog("=====================================");
#endif
        return acks;
    }

    uint32 Peer::GetNextSequence()
    {
        m_packetSequence = m_packetSequence++ % FLUX_MAX_SEND_PACKETS;
        return m_packetSequence;
    }
}