#include "Serializer/Streams/FluxBinaryStream.h"
#include "Serializer/FluxSerializable.h"

namespace Flux
{
    BinaryStream::BinaryStream()
        : m_capacity(BINARY_STREAM_CAPACITY)
        , m_size(0)
        , m_index(0)
    {
        m_buffer = (uint8*)malloc(m_capacity * sizeof(uint8));
    }

    BinaryStream::~BinaryStream()
    {
        free(m_buffer);
        m_buffer = nullptr;
    }

    void BinaryStream::Write(const char* name, uint8 value)
    {
        CopyToBuffer(&value, sizeof(uint8));
    }

    void BinaryStream::Write(const char* name, uint16 value)
    {
        CopyToBuffer(&value, sizeof(uint16));
    }

    void BinaryStream::Write(const char* name, uint32 value)
    {
        CopyToBuffer(&value, sizeof(uint32));
    }

    void BinaryStream::Write(const char* name, uint64 value)
    {
        CopyToBuffer(&value, sizeof(uint64));
    }

    void BinaryStream::Write(const char* name, int8 value)
    {
        CopyToBuffer(&value, sizeof(int8));
    }

    void BinaryStream::Write(const char* name, int16 value)
    {
        CopyToBuffer(&value, sizeof(int16));
    }

    void BinaryStream::Write(const char* name, int32 value)
    {
        CopyToBuffer(&value, sizeof(int32));
    }

    void BinaryStream::Write(const char* name, int64 value)
    {
        CopyToBuffer(&value, sizeof(int64));
    }

    void BinaryStream::Read(const char* name, uint8& value)
    {
        ReadFromBuffer(&value, sizeof(uint8));
    }

    void BinaryStream::Read(const char* name, uint16& value)
    {
        ReadFromBuffer(&value, sizeof(uint16));
    }

    void BinaryStream::Read(const char* name, uint32& value)
    {
        ReadFromBuffer(&value, sizeof(uint32));
    }

    void BinaryStream::Read(const char* name, uint64& value)
    {
        ReadFromBuffer(&value, sizeof(uint64));
    }

    void BinaryStream::Read(const char* name, int8& value)
    {
        ReadFromBuffer(&value, sizeof(int8));
    }

    void BinaryStream::Read(const char* name, int16& value)
    {
        ReadFromBuffer(&value, sizeof(int16));
    }

    void BinaryStream::Read(const char* name, int32& value)
    {
        ReadFromBuffer(&value, sizeof(int32));
    }

    void BinaryStream::Read(const char* name, int64& value)
    {
        ReadFromBuffer(&value, sizeof(int64));
    }

    void BinaryStream::Reset()
    {
        m_index = 0;
    }

    uint32 BinaryStream::GetBuffer(uint8* buffer, uint32 bufferSize)
    {
        if (bufferSize < m_size)
        {
            return 0;
        }

        memcpy(buffer, m_buffer, m_size);
        return m_size;
    }

    Bool BinaryStream::IsEmpty() const
    {
        return (Bool)(m_size == 0);
    }

    uint32 BinaryStream::GetSize() const
    {
        return m_size;
    }

    Bool BinaryStream::LoadFromBuffer(const uint8* buffer, uint32 size)
    {
        if (m_capacity < size)
        {
            m_capacity = size;
            m_buffer = (uint8*)realloc(m_buffer, m_capacity * sizeof(uint8));
        }
        memcpy(m_buffer, buffer, size);
        m_size = size;

        m_index = 0;

        return True;
    }

    void BinaryStream::CopyToBuffer(void* value, uint32 length)
    {
        m_size += length;
        if (m_size > m_capacity)
        {
            m_capacity += BINARY_STREAM_CAPACITY;
            m_buffer = (uint8*)realloc(m_buffer, m_capacity * sizeof(uint8));
        }

        memcpy(m_buffer + m_index, value, length);
        m_index += length;
    }

    void BinaryStream::ReadFromBuffer(void* value, uint32 length)
    {
        memcpy(value, m_buffer + m_index, length);
        m_index += length;
    }
}
