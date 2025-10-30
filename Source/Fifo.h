#pragma once

#include <JuceHeader.h>

template<typename T>
class Fifo
{
public:
    void prepare(int numChannels, int numSamples);
    void prepare(size_t numElements);
    bool push(const T& t);
    bool pull(T& t);
    int getNumAvailableForReading() const;
private:
    static constexpr int Capacity = 30;
    juce::Array<T, Capacity> buffers;
    juce::AbstractFifo fifo {Capacity};
};
