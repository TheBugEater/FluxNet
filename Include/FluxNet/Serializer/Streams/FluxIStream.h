#pragma once
#include "FluxTypes.h"

namespace Flux
{
    class ISerializable;

    class IStream
    {
    public:
        virtual ~IStream() {}

        virtual void    Write(const char* name, uint8 value)                        = 0;
        virtual void    Write(const char* name, uint16 value)                       = 0;
        virtual void    Write(const char* name, uint32 value)                       = 0;
        virtual void    Write(const char* name, uint64 value)                       = 0;
        virtual void    Write(const char* name, int8 value)                         = 0;
        virtual void    Write(const char* name, int16 value)                        = 0;
        virtual void    Write(const char* name, int32 value)                        = 0;
        virtual void    Write(const char* name, int64 value)                        = 0;

        virtual void    WriteArray(const char* name, uint8* value, uint32 length)   = 0;

        template<typename T>
        void    Write(const char* name, T const& value)
        {
            value.Serialize(this);
        }
        template<typename T>
        void    Write(const char* name, T* value)
        {
            value->Serialize(this);
        }

        virtual void    Read(const char* name, uint8& value)                        = 0;
        virtual void    Read(const char* name, uint16& value)                       = 0;
        virtual void    Read(const char* name, uint32& value)                       = 0;
        virtual void    Read(const char* name, uint64& value)                       = 0;
        virtual void    Read(const char* name, int8& value)                         = 0;
        virtual void    Read(const char* name, int16& value)                        = 0;
        virtual void    Read(const char* name, int32& value)                        = 0;
        virtual void    Read(const char* name, int64& value)                        = 0;

        virtual void    ReadArray(const char* name, uint8* value, uint32& length)   = 0;

        template<typename T>
        void    Read(const char* name, T& value)
        {
            value.Deserialize(this);
        }
        template<typename T>
        void    Read(const char* name, T* value)
        {
            value->Deserialize(this);
        }

        // Read without changing the index. 
        virtual Bool    ReadStealthy(void* buffer, uint32 size)          = 0;
        virtual Bool    LoadFromBuffer(const uint8* buffer, uint32 size) = 0;
        virtual void    Reset()                                          = 0;
        virtual uint32  GetSize() const                                  = 0;
        virtual Bool    IsEmpty() const                                  = 0;
        virtual uint32  GetBuffer(uint8* buffer, uint32 bufferSize)      = 0;
    };
}
