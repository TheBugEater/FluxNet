#include "FluxNetEngineDefines.h"
#include "FluxChannel.h"

namespace Flux
{
    class ISerializable;
    class IStream;
    class BinaryStream;
    class INetNotificationHandler;

    struct ChannelData
    {
        uint16  Channel;
        uint16  Count;
        uint16  Sequences[FLUX_MAX_MSGS_PER_CHANNEL];
    };

    struct SentPacket
    {
        ChannelData             Channels[FLUX_MAX_CHANNELS];
        uint32                  Count;
    };

    class Peer
    {
    public:
        Peer(SocketAddressDescriptor const& descriptor);
        virtual ~Peer();

        using PacketRecvQueue = CircularSequenceBuffer<Bool, FLUX_MAX_RECV_PACKETS, True>;
        using PacketSentQueue = CircularSequenceBuffer<SentPacket, FLUX_MAX_SEND_PACKETS, True>;

        Bool                            Send(ISerializable* object);

        void                            CreateOutgoingPacket(IStream* stream);
        void                            ProcessIncomingPacket(IStream* stream);
        void                            ProcessNotifications(INetNotificationHandler* pHandler);

        void                            Update();

        SocketAddressDescriptor const&  GetAddressDescriptor() const;

    private:
        void                            ProcessReceivedAcks(uint16 sequence, uint32 ackBits);
        uint32                          GetLastAckedPackets() const;
        uint32                          GetNextSequence();

        Channel                         m_channel;
        uint16                          m_lastReceivedSequence;

        PacketSentQueue                 m_sentQueue;
        PacketRecvQueue                 m_recvQueue;

        uint16                          m_packetSequence;

        BinaryStream*                   m_pBinaryStream;
        SocketAddressDescriptor         m_address;
    };
}