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
