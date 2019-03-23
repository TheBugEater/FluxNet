#pragma once
#include <unordered_map>
#include "FluxTypes.h"
#include <assert.h>

// Replace key with CRC32 later
namespace Flux
{
    class ClassDescriptor;
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

    private:
        static ClassFactory*    ms_instance;

        std::unordered_map<uint32, ClassDescriptor*>  m_classes;
    };
}
