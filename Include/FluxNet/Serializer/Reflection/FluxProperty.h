#pragma once
#include "FluxTypes.h"
#include "Serializer/Streams/FluxIStream.h"
#include "Serializer/Reflection/FluxClassFactory.h"

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

    template<typename T>
    class PropertyAllocator
    {
    public:
        static T Allocate(const char* name, IStream* stream)
        {
            return T();
        }
    };
    
    template<typename T>
    class PropertyAllocator<T*>
    {
    public:
        static T* Allocate(const char* name, IStream* stream)
        {
            return FluxNew T;
        }
    };

    template<>
    class PropertyAllocator<ISerializable*>
    {
    public:
        static ISerializable* Allocate(const char* name, IStream* stream)
        {
            uint32 classId = 0;
            stream->ReadStealthy(&classId, sizeof(uint32));
            return ClassFactory::Instance()->CreateClass(classId);
        }
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
            *value = PropertyAllocator<PropertyType>::Allocate("", stream);
            stream->Read("", *value);
        }

    private:
        PointerType         m_offset;
    };
}
