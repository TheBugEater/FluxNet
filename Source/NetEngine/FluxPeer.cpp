#include "NetEngine/FluxPeer.h"
#include "NetEngine/FluxNetMessages.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{

    Peer::Peer(SocketAddressDescriptor const& descriptor)
        : m_address(descriptor)
        , m_channel(EChannelType::Reliable_Ordered)
        , m_recentAcks(0)
        , m_packetSequence(0)
        , m_lastReceivedSequence(0)
    {
    }

    void Peer::Send(ISerializable* object)
    {
        m_channel.Send(object);
    }

    void Peer::SerializePacket(IStream* stream)
    {
        Packet currentPacket;
        currentPacket.m_packetSequence = GetNextSequence();
        currentPacket.m_lastReceivedSequence = m_lastReceivedSequence;
        currentPacket.m_ackBits = m_recentAcks;

        BinaryStream messageStream;
        Message* pMessage = m_channel.PopSendQueue();
        while (pMessage)
        {
            pMessage->Serialize(&messageStream);
            pMessage = m_channel.PopSendQueue();
        }
        currentPacket.m_stream.LoadFromBinaryStream(&messageStream);

        currentPacket.Serialize(stream);
    }

    SocketAddressDescriptor const& Peer::GetAddressDescriptor() const
    {
        return m_address;
    }

    uint32 Peer::GetNextSequence()
    {
        m_packetSequence = m_packetSequence++ % MAX_PACKET_SEQUENCE;
        return m_packetSequence;
    }
}