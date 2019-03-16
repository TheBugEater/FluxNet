#pragma once
#include "FluxTypes.h"
#include "FluxSerializable.h"
#include <vector>

class FluxObjectFactory
{
    template<typename T>
    uint32  RegisterObject()
    {
    }
};

namespace Flux
{
    class BaseClass
    {
    public:
        virtual void    Serialize(IStream* stream, ISerializable* object) = 0;
        virtual void    Deserialize(IStream* stream, ISerializable* object) = 0;
    };
}

class Test : public Flux::ISerializable
{
public:
    Test()
    {
    }
    virtual void Serialize(Flux::IStream* stream)
    {
        StaticClass->Serialize(stream, this);
    }

    virtual void Deserialize(Flux::IStream* stream)
    {
        StaticClass->Deserialize(stream, this);
    }
    static class Flux::BaseClass* StaticClass;

    uint32              value;
    uint32              number;
};

/*
BEGIN_FLUX_SERIALIZE(Test)
    REGISTER_FLUX_PROPERTY(&Test::value, "Value")
END_FLUX_SERIALIZE(Test)
*/

namespace Flux
{
    class IProperty
    {
    public:
        virtual void    Serialize(IStream* stream, ISerializable* object) = 0;
        virtual void    Deserialize(IStream* stream, ISerializable* object) = 0;
    };

    template<typename T>
    class Property : public IProperty
    {
    public:
        using PropertyType = T;
        Property(const char* name, uint64 offset)
            : m_offset(offset)
        {
        }

        void    Serialize(IStream* stream, ISerializable* object)
        {
            stream->Write("", *(PropertyType*)((PointerType)object + m_offset));
        }

        void    Deserialize(IStream* stream, ISerializable* object)
        {
            stream->Read("", *(PropertyType*)((PointerType)object + m_offset));
        }

    private:
        PointerType      m_offset;
    };

    class TestClass : public BaseClass
    {
    public:
        using ClassType = Test;
        TestClass()
            : m_registerd(False)
        {
        }

        void    RegisterClass()
        {
            auto valueOffset = (uint32)&((ClassType*)nullptr)->value;
            auto valueProperty = new Property<uint32>("value", valueOffset);
            m_properties.push_back(valueProperty);

            auto numberOffset = (uint32)&((ClassType*)nullptr)->number;
            auto numberProperty = new Property<uint32>("number", numberOffset);
            m_properties.push_back(numberProperty);
             
            m_registerd = True;
        }

        void    Serialize(IStream* stream, ISerializable* object)
        {
            if (!m_registerd)
            {
                RegisterClass();
            }

            for (auto property : m_properties)
            {
                property->Serialize(stream, object);
            }
        }

        void    Deserialize(IStream* stream, ISerializable* object)
        {
            if (!m_registerd)
            {
                RegisterClass();
            }

            for (auto property : m_properties)
            {
                property->Deserialize(stream, object);
            }
        }
    private:
        std::vector<IProperty*>     m_properties;
        Bool                        m_registerd;
    };
}
Flux::TestClass         globalTestClassObj;
Flux::BaseClass*      Test::StaticClass = &globalTestClassObj;

