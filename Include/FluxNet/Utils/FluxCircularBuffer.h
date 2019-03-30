#pragma once
#include "FluxTypes.h"

namespace Flux
{
    template<typename T, uint16 capacity, Bool overwriteOldSequence = True, T TDefault = T()>
    class CircularSequenceBuffer
    {
    public:
        CircularSequenceBuffer()
            : m_pushIndex(0)
            , m_readIndex(0)
        {
            Reset();
        }

        void        Push(T const& element);
        void        Pop();
        T&          Front();

        void        InsertAt(uint16 sequence, T const& element);
        T&          FindAt(uint16 sequence);

        void        Reset();
        Bool        Empty() const;
    private:
        uint16      SequenceRoll(uint16 sequence);

        T           m_queue[capacity];
        uint16      m_pushIndex;
        uint16      m_readIndex;
    };

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    Bool CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::Empty() const
    {
        return m_readIndex == m_pushIndex;
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::Reset()
    {
        for (uint16 i = 0; i < capacity; i++)
        {
            m_queue[i] = TDefault;
        }
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    T& CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::FindAt(uint16 sequence)
    {
        uint16 index = SequenceRoll(sequence);
        return m_queue[index];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::InsertAt(uint16 sequence, T const& element)
    {
        uint16 index = SequenceRoll(sequence);
        m_queue[index] = element;
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    T& CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::Front()
    {
        return m_queue[m_readIndex];
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::Pop()
    {
        // Empty Queue
        if (Empty())
        {
            return;
        }

        m_queue[m_readIndex] = TDefault;
        m_readIndex = SequenceRoll(++m_readIndex);
    }

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    void CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::Push(T const& element)
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

    template<typename T, uint16 capacity, Bool overwriteOldSequence /*= True*/, T TDefault /*= T()*/>
    uint16 CircularSequenceBuffer<T, capacity, overwriteOldSequence, TDefault>::SequenceRoll(uint16 sequence)
    {
        return sequence % capacity;
    }
}
