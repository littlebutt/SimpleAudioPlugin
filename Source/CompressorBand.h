#pragma once

#include <JuceHeader.h>
#include "Params.h"

class CompressorBand
{
public:
    void prepare(juce::dsp::ProcessSpec& spec);
    void update(juce::AudioProcessorValueTreeState& apvts);
    void process(juce::dsp::ProcessContextReplacing<float> context);
private:
    juce::dsp::Compressor<float> compressor;
};
