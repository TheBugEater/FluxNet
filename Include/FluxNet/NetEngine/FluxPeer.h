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
        Channel                 m_channel;

        SocketAddressDescriptor m_address;
    };
}