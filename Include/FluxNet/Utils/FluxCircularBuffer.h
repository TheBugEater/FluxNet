#pragma once
#include "FluxTypes.h"
#include "FluxBitSet.h"

namespace Flux
{
    template<typename T, uint16 capacity, Bool overwriteOldSequence = True>
    class CircularSequenceBuffer
    {
    public:
        CircularSequenceBuffer()
            : m_pushIndex(0)
            , m_readIndex(0)
        {
            Reset();
        }

        void                Push(T const& element);
        void                Pop();
        T const&            Front();

        void                InsertAt(uint16 sequence, T const& element);
        T const&            FindAt(uint16 sequence);

        Bool                IsPresent(uint16 sequence);
        void                Reset();
        Bool                Empty() const;
    private:
        uint16              SequenceRoll(uint16 sequence);

        T                   m_queue[capacity];
        uint16              m_pushIndex;
        uint16              m_readIndex;
        BitSet<capacity>    m_bits;
    };

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    Bool CircularSequenceBuffer<T, capacity, overwriteOldSequence>::IsPresent(uint16 sequence)
    {
        return m_bits.Get(sequence);
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    Bool CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Empty() const
    {
        return m_readIndex == m_pushIndex;
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Reset()
    {
        m_bits.Reset();
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    T const& CircularSequenceBuffer<T, capacity, overwriteOldSequence>::FindAt(uint16 sequence)
    {
        uint16 index = SequenceRoll(sequence);
        return m_queue[index];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::InsertAt(uint16 sequence, T const& element)
    {
        uint16 index = SequenceRoll(sequence);
        if (!overwriteOldSequence && m_bits.Get(index))
        {
            return;
        }
        m_bits.Set(index);
        m_queue[index] = element;
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    T const& CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Front()
    {
        return m_queue[m_readIndex];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Pop()
    {
        if (Empty())
        {
            return;
        }

        m_bits.Clear(m_readIndex);
        m_readIndex = SequenceRoll(++m_readIndex);
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Push(T const& element)
    {
        if (!overwriteOldSequence)
        {
            if (m_readIndex == 0 && m_pushIndex == capacity - 1 ||
                SequenceRoll(m_pushIndex + 1) == m_readIndex)
            {
                return;
            }
        }

        m_bits.Set(m_pushIndex);
        m_queue[m_pushIndex] = element;
        m_pushIndex = SequenceRoll(m_pushIndex + 1);
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    uint16 CircularSequenceBuffer<T, capacity, overwriteOldSequence>::SequenceRoll(uint16 sequence)
    {
        return sequence % capacity;
    }
}
