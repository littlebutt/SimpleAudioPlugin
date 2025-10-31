#pragma once

#include <JuceHeader.h>

#include "Fifo.h"

enum Channel
{
    Right,
    Left
};

template<typename BlockType>
class SingleChannelSampleFifo
{
public:
    SingleChannelSampleFifo(Channel ch);
    void update(const BlockType& buffer);
    void prepare(int bufferSize);
    int getNumCompleteBuffersAvailable() const;
    bool isPrepared() const;
    int getSize() const;
    bool getAudioBuffer(BlockType& buf);
private:
    Channel channelToUse;
    int fifoIndex = 0;
    Fifo<BlockType> audioBufferFifo;
    BlockType bufferToFill;
    juce::Atomic<bool> prepared = false;
    juce::Atomic<int> size = 0;
    void pushNextSampleIntoFifo(float sample);
};
