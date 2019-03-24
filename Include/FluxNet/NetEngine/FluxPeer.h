#include "FluxTypes.h"
#include "FluxChannel.h"

namespace Flux
{
    class ISerializable;
    class IStream;

    class Peer
    {
    public:
        Peer(SocketAddressDescriptor const& descriptor);

        void                            Send(ISerializable* object);

        void                            SerializePacket(IStream* stream);

        SocketAddressDescriptor const&  GetAddressDescriptor() const;

    private:
        uint32                  GetNextSequence();

        Channel                 m_channel;
        uint32                  m_recentAcks;
        uint16                  m_lastReceivedSequence;

        uint16                  m_packetSequence;

        SocketAddressDescriptor m_address;
    };
}