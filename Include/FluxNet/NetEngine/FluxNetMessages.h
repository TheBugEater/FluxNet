#pragma once
#include "Serializer/FluxReflection.h"
#include "Serializer/Streams/FluxBinarySerializableStream.h"
#include <chrono>

namespace Flux
{
    class Message : public ISerializable
    {
        FLUX_CLASS(Message)

    public:
        Message()
            : m_numRetries(0)
        {
        }

        BinarySerializableStream    m_stream;

        uint16          m_sequence;
        uint8           m_channel;

        std::chrono::system_clock::time_point           m_lastSentTime;
        uint16                                          m_numRetries;
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

    class PingMessage : public ISerializable
    {
        FLUX_CLASS(PingMessage)
    };
}
