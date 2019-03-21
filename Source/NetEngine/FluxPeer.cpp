#include "NetEngine/FluxPeer.h"
#include "NetEngine/FluxNetMessages.h"

namespace Flux
{

    Peer::Peer(SocketAddressDescriptor const& descriptor)
        : m_address(descriptor)
        , m_channel(EChannelType::Reliable_Ordered)
    {
    }

    void Peer::Send(ISerializable* object)
    {
        m_channel.Send(object);
    }

    void Peer::SerializePacket(IStream* stream)
    {
        Message* pMessage = m_channel.PopSendQueue();
        if (pMessage)
        {
            pMessage->Serialize(stream);
        }
    }

    SocketAddressDescriptor const& Peer::GetAddressDescriptor() const
    {
        return m_address;
    }

}