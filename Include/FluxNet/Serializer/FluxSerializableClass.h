#pragma once
#include "FluxTypes.h"
#include <vector>

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

    class ClassBase
    {
    public:
        ClassBase(ClassBase* superClass)
            : m_superClass(superClass)
        {
        }
        virtual ~ClassBase() {}

        template<typename TProperty, typename TClass>
        void AddProperty(TProperty TClass::*member)
        {
            auto valueProperty = new Property<TProperty, TClass>(member);
            m_properties.push_back(valueProperty);
        }

        void    Serialize(IStream* stream, ISerializable* object)
        {
            if (m_superClass)
            {
                m_superClass->Serialize(stream, object);
            }

            for (auto property : m_properties)
            {
                property->Serialize(stream, object);
            }
        }

        void    Deserialize(IStream* stream, ISerializable* object)
        {
            if (m_superClass)
            {
                m_superClass->Deserialize(stream, object);
            }

            for (auto property : m_properties)
            {
                property->Deserialize(stream, object);
            }
        }
    private:
        std::vector<IPropertyBase*> m_properties;
        ClassBase*                  m_superClass;
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

#define FLUX_CLASS(CLASS)                                                                               \
public:                                                                                                 \
static class Flux::ClassBase* StaticClass;                                                              \
virtual class Flux::ClassBase* GetClass() { return CLASS::StaticClass; }                                \
void    Serialize(Flux::IStream* stream);                                                               \
void    Deserialize(Flux::IStream* stream);

#define FLUX_BEGIN_CLASS_ROOT(CLASS)                                                                    \
        FLUX_BEGIN_CLASS_IMPL(CLASS, nullptr)

#define FLUX_BEGIN_CLASS(CLASS, Base)                                                                   \
        FLUX_BEGIN_CLASS_IMPL(CLASS, Base::StaticClass)

#define FLUX_BEGIN_CLASS_IMPL(CLASS, Base)                                                              \
class CLASS##Class : public Flux::ClassBase                                                             \
{                                                                                                       \
public:                                                                                                 \
    CLASS##Class()                                                                                      \
        : ClassBase(Base)                                                                               \
    {                                                                                                   \
        RegisterClass();                                                                                \
    }                                                                                                   \
    void    RegisterClass()                                                                             \
    {

#define FLUX_PROPERTY(Member)                                                                           \
        AddProperty(Member);

#define FLUX_CLASS_END(CLASS)                                                                           \
    }                                                                                                   \
};                                                                                                      \
CLASS##Class        Global##CLASS##Object;                                                              \
Flux::ClassBase*    CLASS::StaticClass = &Global##CLASS##Object;                                        \
void CLASS::Serialize(Flux::IStream* stream)                                                            \
{                                                                                                       \
    StaticClass->Serialize(stream, this);                                                               \
}                                                                                                       \
void CLASS::Deserialize(Flux::IStream* stream)                                                          \
{                                                                                                       \
    StaticClass->Deserialize(stream, this);                                                             \
}

//void Reg()
//{
//    int nextVar = AddP(next, member);
//    int b = AddP(a, member);
//}