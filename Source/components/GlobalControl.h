#pragma once

#include <JuceHeader.h>

#include "../Params.h"

class GlobalControl : public juce::Component
{
public:
    GlobalControl(juce::AudioProcessorValueTreeState& apvts);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Slider inGainSlider, lowMidCrossoverSlider, midHighCrossoverSlider, outGainSlider;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    SliderAttachment inGainSliderAttachment, lowMidCrossoverSliderAttachmennt, midHighCrossoverSliderAttachment, outGainSliderAttachment;
};
