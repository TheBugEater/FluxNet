#pragma once
#include "FluxTypes.h"
#include "Serializer/Reflection/FluxClassFactory.h"
#include "Utils/FluxHash.h"
#include "Utils/FluxAllocator.h"

#define FLUX_CLASS(CLASS)                                                                               \
public:                                                                                                 \
friend class CLASS##Class;                                                                              \
static Flux::ClassDescriptor* StaticClass;                                                              \
static constexpr uint32 ClassID = Flux::CRC32(#CLASS);                                                  \
static const char*      GetClassName() { return #CLASS; }                                               \
virtual class Flux::ClassDescriptor* GetClass() const { return CLASS::StaticClass; }                    \
virtual class Flux::ISerializable* Clone() override;                                                    \
void    Serialize(Flux::IStream* stream) const;                                                         \
void    Deserialize(Flux::IStream* stream);

#define FLUX_BEGIN_CLASS_ROOT(CLASS)                                                                    \
        FLUX_BEGIN_CLASS_IMPL(CLASS, nullptr)

#define FLUX_BEGIN_CLASS(CLASS, Base)                                                                   \
        FLUX_BEGIN_CLASS_IMPL(CLASS, Base::StaticClass)

#define FLUX_BEGIN_CLASS_IMPL(CLASS, Base)                                                              \
class CLASS##Class : public Flux::ClassDescriptor                                                       \
{                                                                                                       \
public:                                                                                                 \
    using ClassType = CLASS;                                                                            \
    virtual const char*    GetClassName() const override { return #CLASS; }                             \
    virtual uint32         GetClassID() const  override { return CLASS::ClassID; }                      \
    virtual uint32         GetClassSize() const  override { return sizeof(CLASS); }                     \
    virtual ISerializable* CreateInstance() override                                                    \
    {                                                                                                   \
        return FluxNew ClassType;                                                                       \
    }                                                                                                   \
    CLASS##Class()                                                                                      \
        : ClassDescriptor(Base)                                                                         \
    {                                                                                                   \
        RegisterClass();                                                                                \
        ClassFactory::Instance()->RegisterClass<ClassType>();                                           \
    }                                                                                                   \
    void    RegisterClass()                                                                             \
    {

#define FLUX_PROPERTY(Member)                                                                           \
        AddProperty(&ClassType::Member);

#define FLUX_PROPERTY_DYNAMIC_ARRAY(Member, SizeMember)                                                 \
        AddDynamicArray(&ClassType::Member, &ClassType::SizeMember);

#define FLUX_END_CLASS(CLASS)                                                                           \
    }                                                                                                   \
};                                                                                                      \
CLASS##Class        Global##CLASS##Object;                                                              \
Flux::ClassDescriptor*    CLASS::StaticClass = &Global##CLASS##Object;                                  \
ISerializable* CLASS::Clone()                                                                           \
{                                                                                                       \
    auto object = FluxNew CLASS;                                                                        \
    memcpy(object, this, sizeof(CLASS));                                                                \
    return object;                                                                                      \
}                                                                                                       \
void CLASS::Serialize(Flux::IStream* stream) const                                                      \
{                                                                                                       \
    StaticClass->Serialize(stream, this);                                                               \
}                                                                                                       \
void CLASS::Deserialize(Flux::IStream* stream)                                                          \
{                                                                                                       \
    StaticClass->Deserialize(stream, this);                                                             \
}
