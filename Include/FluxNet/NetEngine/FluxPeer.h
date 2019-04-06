#include "FluxNetEngineDefines.h"
#include "FluxChannel.h"

namespace Flux
{
    class ISerializable;
    class IStream;
    class BinaryStream;
    class INetNotificationHandler;

    class Peer
    {
    public:
        Peer(SocketAddressDescriptor const& descriptor);
        virtual ~Peer();

        using PacketRecvQueue = CircularSequenceBuffer<Bool, FLUX_MAX_RECV_PACKETS, True>;
        using PacketSendQueue = CircularSequenceBuffer<uint32, FLUX_MAX_SEND_PACKETS, True>;

        void                            Send(ISerializable* object);

        void                            CreateOutgoingPacket(IStream* stream);
        void                            ProcessIncomingPacket(IStream* stream);
        void                            ProcessNotifications(INetNotificationHandler* pHandler);

        void                            Update();

        SocketAddressDescriptor const&  GetAddressDescriptor() const;

    private:
        uint32                          GetLastAckedPackets() const;
        uint32                          GetNextSequence();

        Channel                         m_channel;
        uint32                          m_recentAcks;
        uint16                          m_lastReceivedSequence;

        PacketSendQueue                 m_sentQueue;
        PacketRecvQueue                 m_recvQueue;

        uint16                          m_packetSequence;

        BinaryStream*                   m_pBinaryStream;
        SocketAddressDescriptor         m_address;
    };
}