#include "Serializer/Reflection/FluxClassFactory.h"
#include <Serializer/FluxReflection.h>

namespace Flux
{
    ClassFactory::ClassFactory()
    {

    }

    ClassFactory::~ClassFactory()
    {

    }

    ClassFactory* ClassFactory::Instance()
    {
        static ClassFactory factory;
        return &factory;
    }

    ISerializable* ClassFactory::GenerateClassHierachy(IStream* stream)
    {
        uint32 classId;
        stream->ReadStealthy(&classId, sizeof(uint32));

        auto foundIterator = m_classes.find(classId);
        if (foundIterator != m_classes.end())
        {
            auto instance = foundIterator->second->CreateInstance();
            if (instance)
            {
                instance->Deserialize(stream);
                return instance;
            }
        }

        return nullptr;
    }

    ISerializable* ClassFactory::CreateClass(uint32 ClassID)
    {
        auto foundIterator = m_classes.find(ClassID);
        if (foundIterator != m_classes.end())
        {
            auto instance = foundIterator->second->CreateInstance();
            return instance;
        }
        return nullptr;
    }

}