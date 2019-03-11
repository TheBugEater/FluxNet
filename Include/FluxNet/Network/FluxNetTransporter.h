#pragma once
#include "Utils/FluxNetUpdater.h"
#include "FluxTypes.h"
#include "FluxNetDefines.h"
#include "NetEngine/FluxNetEngineDefines.h"

namespace Flux
{
    class NetTransporter : public NetUpdater
    {
    public:

        virtual void            Update() override;

    private:
        
    };
}
