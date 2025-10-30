#include "Fifo.h"


template<typename T>
void Fifo<T>::prepare(int numChannels, int numSamples)
{
    for (auto& buffer : buffers)
    {
        buffer.setSize(numChannels, numSamples,
            false, //clear everything?
            true, //including the extra space?
            true); //avoid reallocating if you can?
        buffer.clear();
    }
}

template<typename T>
void Fifo<T>::prepare(size_t numElements)
{
    for( auto& buffer : buffers )
    {
        buffer.clear();
        buffer.resize(numElements, 0);
    }
}

template<typename T>
bool Fifo<T>::push(const T& t)
{
    auto write = fifo.write(1);
    if( write.blockSize1 > 0 )
    {
        buffers[write.startIndex1] = t;
        return true;
    }
        
    return false;
}

template<typename T>
bool Fifo<T>::pull(T& t)
{
    auto read = fifo.read(1);
    if( read.blockSize1 > 0 )
    {
        t = buffers[read.startIndex1];
        return true;
    }
        
    return false;
}

template<typename T>
int Fifo<T>::getNumAvailableForReading() const
{
    return fifo.getNumReady();
}
