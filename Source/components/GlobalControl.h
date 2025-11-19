#pragma once

#include <JuceHeader.h>

#include "../Params.h"
#include "RotarySliderWithLabels.h"

class GlobalControl : public juce::Component
{
public:
    GlobalControl(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    RotarySliderWithLabels inGainSlider, lowMidCrossoverSlider, midHighCrossoverSlider, outGainSlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    SliderAttachment inGainSliderAttachment, lowMidCrossoverSliderAttachmennt, midHighCrossoverSliderAttachment, outGainSliderAttachment;
};
