#include "Serializer/Reflection/FluxClass.h"

namespace Flux
{
    ClassDescriptor::ClassDescriptor(ClassDescriptor* superClass)
        : m_superClass(superClass)
    {

    }

    void ClassDescriptor::Serialize(IStream* stream, ISerializable* object)
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

    void ClassDescriptor::Deserialize(IStream* stream, ISerializable* object)
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
}
