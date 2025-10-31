#pragma once

#include <JuceHeader.h>

#include "Fifo.h"

enum FFTOrder
{
    order2048 = 11,
    order4096 = 12,
    order8192 = 13
};

template<typename BlockType>
class FFTDataGenerator
{
public:
    void produceFFTDataForRendering(const juce::AudioBuffer<float>& audioData, const float negativeInfinity);
    void changeOrder(FFTOrder newOrder);
    int getFFTSize() const;
    int getNumAvailableFFTDataBlocks() const;
    bool getFFTData(BlockType& fftData);

private:
    FFTOrder order;
    BlockType fftData;
    std::unique_ptr<juce::dsp::FFT> forwardFFT;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> window;
    Fifo<BlockType> fftDataFifo;
};