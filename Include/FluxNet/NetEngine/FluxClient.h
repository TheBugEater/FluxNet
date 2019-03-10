#include "NetEngine/FluxNetEngineDefines.h"
#include "Network/FluxNetUpdater.h"

namespace Flux
{
    class Peer;

    struct ClientConfig
    {
        uint32      ServerIP;
        uint16      ServerPort;
    };

    class Client : public NetUpdater
    {
    public:
        Client(ClientConfig const& config);

        void                    Connect();

        virtual void            Update() override;

    private:
        EPeerState              ProcessMessages();

        Peer*                   m_peer;
        friend class            NetEngine;
    };
}