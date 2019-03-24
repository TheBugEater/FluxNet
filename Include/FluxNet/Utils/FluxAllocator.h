#pragma once
#include "FluxTypes.h"

#define FluxNew new
#define FluxDelete delete

namespace Flux
{
    class Allocator
    {
    public:
        virtual void*   Allocate(uint32 size) = 0;
        virtual void    Free(void* data) = 0;
    };
}
