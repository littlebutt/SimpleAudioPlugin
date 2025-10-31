#include "SingleChannelSampleFifo.h"

template<typename BlockType>
SingleChannelSampleFifo<BlockType>::SingleChannelSampleFifo(Channel ch)
: channelToUse(ch)
{
    prepared.set(false);
}

template<typename BlockType>
void SingleChannelSampleFifo<BlockType>::update(const BlockType& buffer)
{
    auto* channelPtr = buffer.getReadPointer(channelToUse);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        pushNextSampleIntoFifo(channelPtr[i]);
    }
}

template<typename BlockType>
void SingleChannelSampleFifo<BlockType>::prepare(int bufferSize)
{
    prepared.set(false);
    size.set(bufferSize);

    bufferToFill.setSize(1,            //channel
                        bufferSize,    //num samples
                        false,         //keepExistingContent
                        true,          //clear extra space
                        true);         //avoid reallocating
    audioBufferFifo.prepare(1, bufferSize);
    fifoIndex = 0;
    prepared.set(true);
}

template<typename BlockType>
int SingleChannelSampleFifo<BlockType>::getNumCompleteBuffersAvailable() const
{
    return audioBufferFifo.getNumAvailableForReading();
}

template<typename BlockType>
bool SingleChannelSampleFifo<BlockType>::isPrepared() const
{
    return prepared.get();
}

template<typename BlockType>
int SingleChannelSampleFifo<BlockType>::getSize() const
{
    return size.get();
}

template<typename BlockType>
bool SingleChannelSampleFifo<BlockType>::getAudioBuffer(BlockType& buf)
{
    return audioBufferFifo.pull(buf);
}

template<typename BlockType>
void SingleChannelSampleFifo<BlockType>::pushNextSampleIntoFifo(float sample)
{
    if (fifoIndex == bufferToFill.getNumSamples())
    {
        auto ok = audioBufferFifo.push(bufferToFill);
        juce::ignoreUnused(ok);
        fifoIndex = 0;
    }
    bufferToFill.setSample(0, fifoIndex, sample);
    ++ fifoIndex;
}