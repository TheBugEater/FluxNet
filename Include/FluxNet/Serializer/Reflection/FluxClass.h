#pragma once
#include "FluxProperty.h"
#include <vector>

namespace Flux
{
    class IStream;
    class ISerializable;

    class ClassBase
    {
    public:
        ClassBase(ClassBase* superClass);
        virtual ~ClassBase() {}

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
        ClassBase*                  m_superClass;
    };
}