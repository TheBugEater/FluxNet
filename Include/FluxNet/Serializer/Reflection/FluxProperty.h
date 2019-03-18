#pragma once
#include "FluxTypes.h"

namespace Flux
{
    class IStream;
    class ISerializable;

    template<typename TClass, typename TMember>
    static PointerType  GetOffset(TMember TClass::*member)
    {
        return (PointerType)&((TClass*)nullptr->*member) - (PointerType)nullptr;
    }

    class IPropertyBase
    {
    public:
        virtual ~IPropertyBase() {}
        virtual void    Serialize(IStream* stream, ISerializable* object) = 0;
        virtual void    Deserialize(IStream* stream, ISerializable* object) = 0;
    };

    template<typename TProperty, typename TClass>
    class Property : public IPropertyBase
    {
    public:
        using PropertyType = TProperty;
        Property(TProperty TClass::*member)
        {
            m_offset = GetOffset(member);
        }

        virtual void    Serialize(IStream* stream, ISerializable* object)
        {
            PropertyType* value = (PropertyType*)((PointerType)object + m_offset);
            stream->Write("", *value);
        }

        virtual void    Deserialize(IStream* stream, ISerializable* object)
        {
            PropertyType* value = (PropertyType*)((PointerType)object + m_offset);
            stream->Read("", *value);
        }

    private:
        PointerType         m_offset;
    };
}
