#pragma once
#include "FluxTypes.h"
#include "Serializer/Reflection/FluxClassFactory.h"

#define FLUX_CLASS(CLASS)                                                                               \
public:                                                                                                 \
friend class CLASS##Class;                                                                              \
static Flux::ClassBase* StaticClass;                                                                    \
static const char*      GetClassName() { return #CLASS; }                                               \
virtual class Flux::ClassBase* GetClass() const { return CLASS::StaticClass; }                          \
void    Serialize(Flux::IStream* stream);                                                               \
void    Deserialize(Flux::IStream* stream);

#define FLUX_BEGIN_CLASS_ROOT(CLASS)                                                                    \
        FLUX_BEGIN_CLASS_IMPL(CLASS, nullptr)

#define FLUX_BEGIN_CLASS(CLASS, Base)                                                                   \
        FLUX_BEGIN_CLASS_IMPL(CLASS, Base::StaticClass)

#define FLUX_BEGIN_CLASS_IMPL(CLASS, Base)                                                              \
class CLASS##Class : public Flux::ClassBase                                                             \
{                                                                                                       \
public:                                                                                                 \
    using ClassType = CLASS;                                                                            \
    CLASS##Class()                                                                                      \
        : ClassBase(Base)                                                                               \
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
Flux::ClassBase*    CLASS::StaticClass = &Global##CLASS##Object;                                        \
void CLASS::Serialize(Flux::IStream* stream)                                                            \
{                                                                                                       \
    StaticClass->Serialize(stream, this);                                                               \
}                                                                                                       \
void CLASS::Deserialize(Flux::IStream* stream)                                                          \
{                                                                                                       \
    StaticClass->Deserialize(stream, this);                                                             \
}
