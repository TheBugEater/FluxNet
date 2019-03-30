#pragma once
#include "FluxTypes.h"

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
        }

        void        Push(T const& element);
        void        Pop();
        T&          Front();

        void        InsertAt(uint16 sequence, T const& element);
        T&          FindAt(uint16 sequence);

    private:
        uint16      SequenceRoll(uint16 sequence);

        T           m_queue[capacity];
        uint16      m_pushIndex;
        uint16      m_readIndex;
    };

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    T& CircularSequenceBuffer<T, capacity, overwriteOldSequence>::FindAt(uint16 sequence)
    {
        uint16 index = SequenceRoll(sequence);
        return m_queue[index];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::InsertAt(uint16 sequence, T const& element)
    {
        uint16 index = SequenceRoll(sequence);
        m_queue[index] = element;
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    T& CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Front()
    {
        return m_queue[m_readIndex];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence>::Pop()
    {
        // Empty Queue
        if (m_readIndex == m_pushIndex)
        {
            return;
        }

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

        m_queue[m_pushIndex] = element;
        m_pushIndex = SequenceRoll(m_pushIndex + 1);
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence>
    uint16 CircularSequenceBuffer<T, capacity, overwriteOldSequence>::SequenceRoll(uint16 sequence)
    {
        return sequence % capacity;
    }
}
