#pragma once
#include "FluxProperty.h"
#include <vector>

namespace Flux
{
    class IStream;
    class ISerializable;

    class ClassDescriptor
    {
    public:
        ClassDescriptor(ClassDescriptor* superClass);
        virtual ~ClassDescriptor() {}

        template<typename TProperty, typename TClass>
        void AddProperty(TProperty TClass::*member)
        {
            auto valueProperty = new Property<TProperty, TClass>(member);
            m_properties.push_back(valueProperty);
        }

        void    Serialize(IStream* stream, ISerializable* object);
        void    Deserialize(IStream* stream, ISerializable* object);

    private:
        std::vector<IPropertyBase*> m_properties;
        ClassDescriptor*                  m_superClass;
    };
}