#include "NetEngine/FluxNetEngineDefines.h"
#include "Utils/FluxNetUpdater.h"
#include "FluxNetDefines.h"
#include <string>

namespace Flux
{
    class Peer;

    struct ClientConfig
    {
        ESocketFamily   Family;
        std::string     ServerIP;
        uint16          ServerPort;
    };

    class Client : public NetUpdater
    {
    private:
        Client(ClientConfig const& config);
        ~Client();

        virtual void            Update() override;
        void                    FlushSend();
    public:

        Bool                    Connect();


    private:
        Peer*                   m_peer;
        SocketDescriptor        m_socket;

        uint8                   m_recvBuffer[FLUX_NET_MTU];
        SocketAddressDescriptor m_recvAddress;

        ClientConfig            m_config;

        friend class            NetEngine;
    };
}