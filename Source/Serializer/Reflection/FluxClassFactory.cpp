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

    ISerializable* ClassFactory::CreateClass(IStream* stream)
    {
        uint32 classId;
        stream->Read("", classId);
        stream->Reset();

        auto foundIterator = m_classes.find(classId);
        if (foundIterator != m_classes.end())
        {
            auto instance = foundIterator->second->CreateInstance();
            instance->Deserialize(stream);
            return instance;
        }

        return nullptr;
    }
}