#pragma once

namespace Flux
{
    class ISerializable;
    class Peer;
    class INetNotificationHandler
    {
    public:
        virtual void OnMessage(Peer* pPeer, ISerializable*   pMessage) = 0;
    };
};
