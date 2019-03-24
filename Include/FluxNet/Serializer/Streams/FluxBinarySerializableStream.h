#pragma once
#include "Serializer/FluxReflection.h"

namespace Flux
{
    class BinaryStream;

    class BinarySerializableStream : public ISerializable
    {
        FLUX_CLASS(BinarySerializableStream)

    public:
        BinarySerializableStream();
        ~BinarySerializableStream();

        BinarySerializableStream(const BinarySerializableStream& stream);
        BinarySerializableStream const& operator=(const BinarySerializableStream& stream);

        BinarySerializableStream(BinarySerializableStream&& stream);
        BinarySerializableStream const& operator=(BinarySerializableStream&& stream);

        Bool        LoadFromBinaryStream(BinaryStream* stream);
        Bool        LoadToBinaryStream(BinaryStream* stream);

    private:
        uint8*      m_buffer;
        uint32      m_bufferLength;
    };
}