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

        Bool        LoadFromBinaryStream(BinaryStream* stream);
        Bool        LoadToBinaryStream(BinaryStream* stream);

    private:
        uint8*      m_buffer;
        uint32      m_bufferLength;
    };
}