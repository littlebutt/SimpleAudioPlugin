#pragma once

#include <JuceHeader.h>

#include "./RotarySliderWithLabels.h"
#include "../Params.h"

class CompressorBandControl : public juce::Component
{
public:
    CompressorBandControl(juce::AudioProcessorValueTreeState& apvts);
    ~CompressorBandControl();
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    enum BandType
    {
        Low,
        Mid,
        High
    };
    RotarySliderWithLabels attackSlider, releaseSlider, thresholdSlider, ratioSlider;
    juce::ToggleButton lowBand, midBand, highBand;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> attackSliderAttachment, releaseSliderAttachment, thresholdSliderAttachment, ratioSliderAttachment;
    juce::AudioProcessorValueTreeState& apvts;
    LookAndFeel lnf;
    juce::Component::SafePointer<CompressorBandControl> safePtr { this };
    void updateAttachments();
};