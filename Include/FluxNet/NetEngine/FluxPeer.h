#include "FluxTypes.h"
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

        void                            Send(ISerializable* object);

        void                            CreateOutgoingPacket(IStream* stream);
        void                            ProcessIncomingPacket(IStream* stream);
        void                            ProcessNotifications(INetNotificationHandler* pHandler);

        SocketAddressDescriptor const&  GetAddressDescriptor() const;

    private:
        uint32                          GetNextSequence();

        Channel                         m_channel;
        uint32                          m_recentAcks;
        uint16                          m_lastReceivedSequence;

        uint16                          m_packetSequence;

        BinaryStream*                   m_pBinaryStream;
        SocketAddressDescriptor         m_address;
    };
}