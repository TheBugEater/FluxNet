#pragma once
#include <unordered_map>
#include "FluxTypes.h"

// Replace key with CRC32 later
namespace Flux
{
    class ClassBase;
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
            m_classes[TClass::GetClassName()] = TClass::StaticClass;
        }

    private:
        static ClassFactory*    ms_instance;

        std::unordered_map<std::string, ClassBase*>  m_classes;
    };
}
