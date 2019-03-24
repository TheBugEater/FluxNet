#include "Serializer/Reflection/FluxClassDescriptor.h"

namespace Flux
{
    ClassDescriptor::ClassDescriptor(ClassDescriptor* superClass)
        : m_superClass(superClass)
    {

    }

    void ClassDescriptor::Serialize(IStream* stream, ISerializable const* object)
    {
        if (m_superClass)
        {
            m_superClass->Serialize(stream, object);
        }

        stream->Write(GetClassName(), GetClassID());
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

        uint32 ReadValue = 0;
        stream->Read(GetClassName(), ReadValue);
        for (auto property : m_properties)
        {
            property->Deserialize(stream, object);
        }
    }
}
