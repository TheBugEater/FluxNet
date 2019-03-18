#pragma once

namespace Flux
{
    class IStream;
    class ClassBase;

    class ISerializable
    {
    public:
        virtual ClassBase* GetClass() const         = 0;
        virtual void Serialize(IStream* stream)     = 0;
        virtual void Deserialize(IStream* stream)   = 0;
    };
}
