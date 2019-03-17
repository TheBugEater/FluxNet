//#pragma once
//#include "FluxTypes.h"
//#include "FluxSerializable.h"
//#include <vector>
//
//class FluxObjectFactory
//{
//    template<typename T>
//    uint32  RegisterObject()
//    {
//    }
//};
//
//template<typename TClass, typename TMember>
//static PointerType  GetOffset(TMember TClass::*member)
//{
//    return (PointerType)&((TClass*)nullptr->*member) - (PointerType)nullptr;
//}
//
//namespace Flux
//{
//    class BaseClass
//    {
//    public:
//        BaseClass(BaseClass* superClass)
//            : SuperClass(superClass)
//        {
//        }
//        virtual void    Serialize(IStream* stream, ISerializable* object) = 0;
//        virtual void    Deserialize(IStream* stream, ISerializable* object) = 0;
//    private:
//        BaseClass*  SuperClass;
//    };
//}
//
//class Test : public Flux::ISerializable
//{
//public:
//    Test()
//    {
//    }
//    virtual void Serialize(Flux::IStream* stream)
//    {
//        StaticClass->Serialize(stream, this);
//    }
//
//    virtual void Deserialize(Flux::IStream* stream)
//    {
//        StaticClass->Deserialize(stream, this);
//    }
//    static class Flux::BaseClass* StaticClass;
//
//    uint32              value;
//    uint32              number;
//};
//
///*
//BEGIN_FLUX_SERIALIZE(Test)
//    REGISTER_FLUX_PROPERTY(&Test::value, "Value")
//END_FLUX_SERIALIZE(Test)
//*/
//
//namespace Flux
//{
//    class IProperty
//    {
//    public:
//        virtual void    Serialize(IStream* stream, ISerializable* object) = 0;
//        virtual void    Deserialize(IStream* stream, ISerializable* object) = 0;
//    };
//
//    template<typename TProperty>
//    class Property : public IProperty
//    {
//    public:
//        using PropertyType = TProperty;
//        Property(uint64 offset)
//            : m_offset(offset)
//        {
//        }
//
//        void    Serialize(IStream* stream, ISerializable* object)
//        {
//            stream->Write("", *(PropertyType*)((PointerType)object + m_offset));
//        }
//
//        void    Deserialize(IStream* stream, ISerializable* object)
//        {
//            stream->Read("", *(PropertyType*)((PointerType)object + m_offset));
//        }
//
//    private:
//        PointerType      m_offset;
//    };
//
//    class TestClass : public BaseClass
//    {
//    public:
//        using ClassType = Test;
//        TestClass()
//            : BaseClass(nullptr)
//            , m_registerd(False)
//        {
//        }
//
//        void    RegisterClass()
//        {
//            AddProperty(&Test::value);
//            AddProperty(&Test::number);
//
//            m_registerd = True;
//        }
//
//    private:
//        std::vector<IProperty*>     m_properties;
//        Bool                        m_registerd;
//    };
//}
//Flux::TestClass         globalTestClassObj;
//Flux::BaseClass*      Test::StaticClass = &globalTestClassObj;
//
