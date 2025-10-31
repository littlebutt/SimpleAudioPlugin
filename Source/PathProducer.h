#pragma once

#include <JuceHeader.h>

#include "SingleChannelSampleFifo.h"
#include "FFTDataGenerator.h"
#include "AnalyzerPathGenerator.h"

class PathProducer
{
public:
    PathProducer(SingleChannelSampleFifo<juce::AudioBuffer<float>>& scsf);
    void process(juce::Rectangle<float> ffyBounds, double sampleRate);
    juce::Path getPath();

private:
    SingleChannelSampleFifo<juce::AudioBuffer<float>>* leftChannelFifo;
    juce::AudioBuffer<float> monoBuffer;
    FFTDataGenerator<std::vector<float>> leftChannelFFTDataGenerator;
    AnalyzerPathGenerator<juce::Path> pathProducer;
    juce::Path leftChannelFFTPath;
};
