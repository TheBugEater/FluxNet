#pragma once
#include "FluxTypes.h"

namespace Flux
{
    template<typename T, uint32 capacity>
    class StaticQueue
    {
    public:
    private:
        T           m_queue[capacity];
    };
}
