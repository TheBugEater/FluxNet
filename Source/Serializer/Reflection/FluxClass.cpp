#include "Serializer/Reflection/FluxClass.h"

namespace Flux
{
    ClassBase::ClassBase(ClassBase* superClass)
        : m_superClass(superClass)
    {

    }

    void ClassBase::Serialize(IStream* stream, ISerializable* object)
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

    void ClassBase::Deserialize(IStream* stream, ISerializable* object)
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
