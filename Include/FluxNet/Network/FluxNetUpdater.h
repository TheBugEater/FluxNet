#pragma once

namespace Flux
{
    class NetUpdater
    {
    public:
        virtual ~NetUpdater() {}

        virtual void        Update() = 0;
    };
}
