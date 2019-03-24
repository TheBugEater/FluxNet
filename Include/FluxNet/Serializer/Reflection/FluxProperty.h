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
        virtual void    Serialize(IStream* stream, ISerializable const* object) = 0;
        virtual void    Deserialize(IStream* stream, ISerializable* object)     = 0;
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

        virtual void    Serialize(IStream* stream, ISerializable const* object)
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

    template<typename TProperty, typename TClass>
    class DynamicArrayProperty : public IPropertyBase
    {
    public:
        using PropertyType = TProperty;
        using PropertyPointerType = TProperty*;
        DynamicArrayProperty(TProperty* TClass::*member, uint32 TClass::*lengthMember)
        {
            m_offsetBuffer = GetOffset(member);
            m_offsetLength = GetOffset(lengthMember);
        }

        virtual void    Serialize(IStream* stream, ISerializable const* object)
        {
            uint32* lengthValue = (uint32*)((PointerType)object + m_offsetLength);
            PropertyPointerType* value = (PropertyPointerType*)((PointerType)object + m_offsetBuffer);

            stream->WriteArray("", *value, *lengthValue);
        }

        virtual void    Deserialize(IStream* stream, ISerializable* object)
        {
            uint32* lengthValue = (uint32*)((PointerType)object + m_offsetLength);
            PropertyPointerType* value = (PropertyPointerType*)((PointerType)object + m_offsetBuffer);

            // Allocate
            uint32 bufferLength = 0;
            stream->ReadStealthy(&bufferLength, sizeof(uint32));
            *value = FluxNew PropertyType[bufferLength];

            stream->ReadArray("", *value, *lengthValue);
        }

    private:
        PointerType         m_offsetBuffer;
        PointerType         m_offsetLength;
    };
}
