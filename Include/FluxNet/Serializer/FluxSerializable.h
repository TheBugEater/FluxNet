#pragma once

namespace Flux
{
    class IStream;
    class ClassDescriptor;

    class ISerializable
    {
    public:
        virtual ClassDescriptor* GetClass() const   = 0;
        virtual void Serialize(IStream* stream)     = 0;
        virtual void Deserialize(IStream* stream)   = 0;
        virtual ISerializable* Clone()              = 0;
    };
}
