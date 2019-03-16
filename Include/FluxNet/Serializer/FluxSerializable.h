#pragma once

namespace Flux
{
    class IStream;
    class ISerializable
    {
    public:
        virtual void Serialize(IStream* stream) = 0;
        virtual void Deserialize(IStream* stream) = 0;
    };
}
