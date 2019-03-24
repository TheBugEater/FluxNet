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

        virtual ISerializable*      CreateInstance() = 0;
        virtual const char*         GetClassName() const = 0;
        virtual uint32              GetClassID() const = 0;
        virtual uint32              GetClassSize() const = 0;

        template<typename TProperty, typename TClass>
        void AddProperty(TProperty TClass::*member)
        {
            auto valueProperty = FluxNew Property<TProperty, TClass>(member);
            m_properties.push_back(valueProperty);
        }

        template<typename TProperty, typename TClass>
        void AddDynamicArray(TProperty* TClass::*member, uint32 TClass::*lengthMember)
        {
            auto valueProperty = FluxNew DynamicArrayProperty<TProperty, TClass>(member, lengthMember);
            m_properties.push_back(valueProperty);
        }

        void    Serialize(IStream* stream, ISerializable const* object);
        void    Deserialize(IStream* stream, ISerializable* object);

    private:
        std::vector<IPropertyBase*> m_properties;
        ClassDescriptor*                  m_superClass;
    };
}