#include "NetEngine/FluxPeer.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{

    Peer::Peer(SocketAddressDescriptor const& descriptor)
        : m_address(descriptor)
        , m_channel(this, EChannelType::Reliable_Ordered)
        , m_recentAcks(0)
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

    void Peer::Send(ISerializable* object)
    {
        m_channel.Send(object);
    }

    void Peer::CreateOutgoingPacket(IStream* stream)
    {
        Message* pMessage = m_channel.PopOutgoingMessage();
        if (!pMessage)
        {
            return;
        }

        auto currentTime = std::chrono::system_clock::now();
        m_pBinaryStream->Reset();
        while (pMessage)
        {
            pMessage->m_lastSentTime = currentTime;
            pMessage->Serialize(m_pBinaryStream);
            pMessage = m_channel.PopOutgoingMessage();
        }

        Packet currentPacket;
        currentPacket.m_packetSequence = GetNextSequence();
        currentPacket.m_lastReceivedSequence = m_lastReceivedSequence;
        currentPacket.m_ackBits = m_recentAcks;
        currentPacket.m_stream.LoadFromBinaryStream(m_pBinaryStream);

        currentPacket.Serialize(stream);
    }

    void Peer::ProcessIncomingPacket(IStream* stream)
    {
        auto message = ClassFactory::Instance()->GenerateClassHierachyType<Packet>(stream);
        if (message)
        {
            m_recvQueue.InsertAt(message->m_packetSequence, True);
            m_lastReceivedSequence = message->m_packetSequence;

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

    uint32 Peer::GetLastAckedPackets() const
    {
        // Generate 32 Acks from last received
        return 0;
    }

    uint32 Peer::GetNextSequence()
    {
        m_packetSequence = m_packetSequence++ % MAX_PACKET_SEQUENCE;
        return m_packetSequence;
    }
}