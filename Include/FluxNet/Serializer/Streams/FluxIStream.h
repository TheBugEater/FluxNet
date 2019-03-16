#pragma once
#include "FluxTypes.h"

namespace Flux
{
    class ISerializable;

    class IStream
    {
    public:
        virtual ~IStream() {}

        virtual void    Write(const char* name, uint8 value)            = 0;
        virtual void    Write(const char* name, uint16 value)           = 0;
        virtual void    Write(const char* name, uint32 value)           = 0;
        virtual void    Write(const char* name, uint64 value)           = 0;
        virtual void    Write(const char* name, int8 value)             = 0;
        virtual void    Write(const char* name, int16 value)            = 0;
        virtual void    Write(const char* name, int32 value)            = 0;
        virtual void    Write(const char* name, int64 value)            = 0;
        virtual void    Write(const char* name, ISerializable* value)   = 0;

        virtual void    Read(const char* name, uint8& value)            = 0;
        virtual void    Read(const char* name, uint16& value)           = 0;
        virtual void    Read(const char* name, uint32& value)           = 0;
        virtual void    Read(const char* name, uint64& value)           = 0;
        virtual void    Read(const char* name, int8& value)             = 0;
        virtual void    Read(const char* name, int16& value)            = 0;
        virtual void    Read(const char* name, int32& value)            = 0;
        virtual void    Read(const char* name, int64& value)            = 0;
        virtual void    Read(const char* name, ISerializable* value)    = 0;
    };
}
