/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "components/RotarySliderWithLabels.h"

//==============================================================================
/**
*/
class SimpleAudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleAudioPluginAudioProcessorEditor (SimpleAudioPluginAudioProcessor&);
    ~SimpleAudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleAudioPluginAudioProcessor& audioProcessor;

    //==============================================================================
    RotarySliderWithLabels peakFreqSlider,
                            peakGainSlider,
                            peakQualitySlider,
                            lowCutFreqSlider,
                            highCutFreqSlider,
                            lowCutSlopeSlider,
                            highCutSlopeSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;

    SliderAttachment peakFreqSliderAttachment,
                peakGainSliderAttachment,
                peakQualitySliderAttachment,
                lowCutFreqSliderAttachment,
                highCutFreqSliderAttachment,
                lowCutSlopeSliderAttachment,
                highCutSlopeSliderAttachment;

    LookAndFeel lnf;

    /*
    Return the components to paint in the plugin.

    @return The array of the components.
     */
    juce::Array<juce::Component*> getComps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleAudioPluginAudioProcessorEditor)
};
