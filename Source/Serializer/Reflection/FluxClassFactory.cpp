#include "Serializer/Reflection/FluxClassFactory.h"

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
}