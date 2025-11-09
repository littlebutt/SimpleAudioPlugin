#pragma once

#include <JuceHeader.h>
#include "Params.h"

class CompressorBand
{
public:
    void prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock);
    void update(juce::AudioProcessorValueTreeState& apvts);
    void process(juce::AudioSampleBuffer& buffer);
private:
    std::array<juce::dsp::Compressor<float>,3> compressors;

    using LinkwizRileyFilter = juce::dsp::LinkwitzRileyFilter<float>;
    LinkwizRileyFilter LP1, AP2,
                        HP1, LP2,
                            HP2;
    juce::AudioParameterFloat* lowMidCrossover {nullptr};
    juce::AudioParameterFloat* midHighCrossover {nullptr};
    std::array<juce::AudioBuffer<float>, 3> filterBuffers;
    juce::dsp::Gain<float> inputGain, outputGain;
    juce::AudioParameterFloat* inputGainParam {nullptr};
    juce::AudioParameterFloat* outputGainParam {nullptr};

    void applyGain(juce::AudioSampleBuffer& buffer, juce::dsp::Gain<float>& gain);
};
