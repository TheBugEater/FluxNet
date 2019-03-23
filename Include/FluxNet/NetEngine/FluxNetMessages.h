#pragma once
#include "Serializer/FluxReflection.h"

namespace Flux
{
    class Message : public ISerializable
    {
        FLUX_CLASS(Message)

    public:
        uint8*          m_buffer;
        uint32          m_bufferLength;
        ISerializable*  m_serializable;

        uint16          m_sequence;
        uint8           m_channel;
    };

    class PacketHeader : public ISerializable
    {
        FLUX_CLASS(PacketHeader)

    public:
        uint32          m_ackBits;
        uint16          m_currentAck;
    };

    class ConnectMessage : public ISerializable
    {
        FLUX_CLASS(ConnectMessage)

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
