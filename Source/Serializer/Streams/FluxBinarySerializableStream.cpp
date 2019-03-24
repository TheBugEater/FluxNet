#include "Serializer/Streams/FluxBinarySerializableStream.h"
#include "Serializer/Streams/FluxBinaryStream.h"

namespace Flux
{
    FLUX_BEGIN_CLASS_ROOT(BinarySerializableStream)
        FLUX_PROPERTY_DYNAMIC_ARRAY(m_buffer, m_bufferLength)
    FLUX_END_CLASS(BinarySerializableStream)

    BinarySerializableStream::BinarySerializableStream()
        : m_buffer(nullptr)
        , m_bufferLength(0)
    {
    }

    BinarySerializableStream::BinarySerializableStream(const BinarySerializableStream& stream)
        : m_buffer(nullptr)
        , m_bufferLength(0)
    {
        if (stream.m_bufferLength > 0)
        {
            m_bufferLength = stream.m_bufferLength;

            m_buffer = FluxNew uint8[m_bufferLength];
            memcpy(m_buffer, stream.m_buffer, m_bufferLength);
        }
    }

    BinarySerializableStream::BinarySerializableStream(BinarySerializableStream&& stream)
    {
        m_buffer = stream.m_buffer;
        m_bufferLength = stream.m_bufferLength;
        
        stream.m_buffer = nullptr;
        stream.m_bufferLength = 0;
    }

    BinarySerializableStream const& BinarySerializableStream::operator=(BinarySerializableStream&& stream)
    {
        m_buffer = stream.m_buffer;
        m_bufferLength = stream.m_bufferLength;

        stream.m_buffer = nullptr;
        stream.m_bufferLength = 0;

        return *this;
    }

    BinarySerializableStream const& BinarySerializableStream::operator=(const BinarySerializableStream& stream)
    {
        if (stream.m_bufferLength > 0)
        {
            m_bufferLength = stream.m_bufferLength;

            m_buffer = FluxNew uint8[m_bufferLength];
            memcpy(m_buffer, stream.m_buffer, m_bufferLength);
        }
        return *this;
    }

    BinarySerializableStream::~BinarySerializableStream()
    {
        if (m_buffer)
        {
            FluxDelete [] m_buffer;
            m_buffer = nullptr;
        }
    }

    Bool BinarySerializableStream::LoadFromBinaryStream(BinaryStream* stream)
    {
        m_bufferLength = stream->GetSize();
        m_buffer = FluxNew uint8[m_bufferLength];
        stream->GetBuffer(m_buffer, m_bufferLength);

        return True;
    }

    Bool BinarySerializableStream::LoadToBinaryStream(BinaryStream* stream)
    {
        return stream->LoadFromBuffer(m_buffer, m_bufferLength);
    }
}
