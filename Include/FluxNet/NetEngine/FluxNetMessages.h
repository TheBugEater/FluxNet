#pragma once
#include "Serializer/FluxReflection.h"
#include "Serializer/Streams/FluxBinarySerializableStream.h"

namespace Flux
{
    class Message : public ISerializable
    {
        FLUX_CLASS(Message)

    public:
        BinarySerializableStream    m_stream;

        uint16          m_sequence;
        uint8           m_channel;
    };

    class Packet : public ISerializable
    {
        FLUX_CLASS(Packet)

    public:
        uint32                      m_ackBits;
        uint16                      m_lastReceivedSequence;
        uint16                      m_packetSequence;
        BinarySerializableStream    m_stream;
    };

    class HelloMessage : public ISerializable
    {
        FLUX_CLASS(HelloMessage)

    public:
        uint32              m_magicNumber;
    };

    class TestMessage : public ISerializable
    {
        FLUX_CLASS(TestMessage)

    public:
        uint32              m_value;
    };
}
