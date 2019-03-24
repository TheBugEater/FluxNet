#pragma once
#include <unordered_map>
#include "FluxTypes.h"

// Replace key with CRC32 later
namespace Flux
{
    class ClassDescriptor;
    class ISerializable;
    class IStream;

    class ClassFactory
    {
    private:
        ClassFactory();
        ~ClassFactory();
    public:
        static ClassFactory*    Instance();

        template<typename TClass>
        void    RegisterClass()
        {
            assert(m_classes.find(TClass::ClassID) == m_classes.end() && "CRC32 Collision or Registering the Same Class Twice");

            m_classes[TClass::ClassID] = TClass::StaticClass;
        }

        template<typename T>
        T*  GenerateClassHierachyType(IStream* stream)
        {
            uint32 classId;
            stream->ReadStealthy(&classId, sizeof(uint32));
            if (T::StaticClass->GetClassID() == classId)
            {
                return static_cast<T*>(GenerateClassHierachy(stream));
            }
            return nullptr;
        }

        ISerializable*  GenerateClassHierachy(IStream* stream);

        ISerializable*  CreateClass(uint32 ClassID);

    private:
        static ClassFactory*    ms_instance;

        std::unordered_map<uint32, ClassDescriptor*>  m_classes;
    };
}
