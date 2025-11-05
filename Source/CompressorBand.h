#pragma once

#include <JuceHeader.h>
#include "Params.h"

class CompressorBand
{
public:
    void prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock);
    void update(juce::AudioProcessorValueTreeState& apvts);
    void process(juce::dsp::ProcessContextReplacing<float> context);
    void process(juce::AudioSampleBuffer& buffer);
private:
    juce::dsp::Compressor<float> compressor;

    using LinkwizRileyFilter = juce::dsp::LinkwitzRileyFilter<float>;
    LinkwizRileyFilter LP, HP;
    juce::AudioParameterFloat* lowCrossover {nullptr};
    std::array<juce::AudioBuffer<float>, 2> filterBuffers;
};
