#pragma once
#include "FluxTypes.h"
#include "Serializer/Reflection/FluxClassFactory.h"
#include "Utils/FluxHash.h"
#include "Utils/FluxNetAllocator.h"

#define FLUX_CLASS(CLASS)                                                                               \
public:                                                                                                 \
friend class CLASS##Class;                                                                              \
static Flux::ClassDescriptor* StaticClass;                                                              \
static constexpr uint32 ClassID = Flux::CRC32(#CLASS);                                                  \
static const char*      GetClassName() { return #CLASS; }                                               \
virtual class Flux::ClassDescriptor* GetClass() const { return CLASS::StaticClass; }                    \
void    Serialize(Flux::IStream* stream);                                                               \
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
    ISerializable* CreateInstance()                                                                     \
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

#define FLUX_END_CLASS(CLASS)                                                                           \
    }                                                                                                   \
};                                                                                                      \
CLASS##Class        Global##CLASS##Object;                                                              \
Flux::ClassDescriptor*    CLASS::StaticClass = &Global##CLASS##Object;                                  \
void CLASS::Serialize(Flux::IStream* stream)                                                            \
{                                                                                                       \
    stream->Write(#CLASS, CLASS::ClassID);                                                              \
    StaticClass->Serialize(stream, this);                                                               \
}                                                                                                       \
void CLASS::Deserialize(Flux::IStream* stream)                                                          \
{                                                                                                       \
    uint32 classId = 0;                                                                                 \
    stream->Read(#CLASS, classId);                                                                      \
    assert(classId == CLASS::ClassID);                                                                  \
    StaticClass->Deserialize(stream, this);                                                             \
}
