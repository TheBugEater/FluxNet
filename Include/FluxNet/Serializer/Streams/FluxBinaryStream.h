#pragma once
#include "Serializer/Streams/FluxIStream.h"

#define BINARY_STREAM_CAPACITY 1024

namespace Flux
{
    class BinaryStream : public IStream
    {
    public:
        BinaryStream();
        ~BinaryStream();

        virtual void Write(const char* name, uint8 value)           override;
        virtual void Write(const char* name, uint16 value)          override;
        virtual void Write(const char* name, uint32 value)          override;
        virtual void Write(const char* name, uint64 value)          override;
        virtual void Write(const char* name, int8 value)            override;
        virtual void Write(const char* name, int16 value)           override;
        virtual void Write(const char* name, int32 value)           override;
        virtual void Write(const char* name, int64 value)           override;

        virtual void Read(const char* name, uint8& value)           override;
        virtual void Read(const char* name, uint16& value)          override;
        virtual void Read(const char* name, uint32& value)          override;
        virtual void Read(const char* name, uint64& value)          override;
        virtual void Read(const char* name, int8& value)            override;
        virtual void Read(const char* name, int16& value)           override;
        virtual void Read(const char* name, int32& value)           override;
        virtual void Read(const char* name, int64& value)           override;

        virtual void Reset()                                        override;

        virtual uint32  GetBuffer(uint8* buffer, uint32 bufferSize) override;

        virtual Bool    IsEmpty() const                             override;
        virtual uint32  GetSize() const                             override;


        virtual Bool LoadFromBuffer(const uint8* buffer, uint32 size) override;

    private:
        void            CopyToBuffer(void* value, uint32 length);
        void            ReadFromBuffer(void* value, uint32 length);

        uint32          m_capacity;
        uint32          m_size;
        uint32          m_index;
        uint8*          m_buffer;
    };
}