#pragma once
#include "FluxTypes.h"

namespace Flux
{
    template<uint32 size>
    class BitSet
    {
    public:
        BitSet()
            : m_bits()
        {
        }

        Bool                    Get(uint32 index);
        void                    Set(uint32 index);
        void                    Clear(uint32 index);
        void                    Reset();

    private:
        static constexpr uint32 BitsInUINT32 = sizeof(uint32) * CHAR_BIT;
        static constexpr uint32 NumInts = size / BitsInUINT32;

        uint32&                 GetIntAtIndex(uint32 index);
        uint32                  GetBitIndex(uint32 index);

        uint32                  m_bits[NumInts];
    };

    template<uint32 size>
    void Flux::BitSet<size>::Reset()
    {
        memset(m_bits, 0, sizeof(uint32) * NumInts);
    }

    template<uint32 size>
    uint32 Flux::BitSet<size>::GetBitIndex(uint32 index)
    {
        uint32 bit = index % BitsInUINT32;
        return bit;
    }

    template<uint32 size>
    uint32& Flux::BitSet<size>::GetIntAtIndex(uint32 index)
    {
        uint32& indexedInt = m_bits[index / BitsInUINT32];
        return indexedInt;
    }

    template<uint32 size>
    void BitSet<size>::Clear(uint32 index)
    {
        if (index >= size)
        {
            return;
        }
        uint32& indexedInt = GetIntAtIndex(index);
        uint32 bit = GetBitIndex(index);
        indexedInt &= ~(1 << bit);
    }

    template<uint32 size>
    void BitSet<size>::Set(uint32 index)
    {
        if (index >= size)
        {
            return;
        }

        uint32& indexedInt  = GetIntAtIndex(index);
        uint32 bit          = GetBitIndex(index);
        indexedInt |= 1 << bit;
    }

    template<uint32 size>
    Bool BitSet<size>::Get(uint32 index)
    {
        if (index >= size)
        {
            return False;
        }

        uint32& indexedInt = GetIntAtIndex(index);
        uint32 bit = GetBitIndex(index);
        if (indexedInt & (1 << bit))
        {
            return True;
        }

        return False;
    }

}
