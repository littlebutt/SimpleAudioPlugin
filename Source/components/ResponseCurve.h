#pragma once

#include <JuceHeader.h>

class SimpleAudioPluginAudioProcessor;

class ResponseCurve : public juce::Component
{
public:
    ResponseCurve(SimpleAudioPluginAudioProcessor&);
    ~ResponseCurve();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    SimpleAudioPluginAudioProcessor& audioProcessor;

    void drawTextLabels(juce::Graphics& g);

    juce::Rectangle<int> getRenderArea();
    juce::Rectangle<int> getAnalysisArea();

    //=============================================================
    juce::Array<float> freqs = {
        20, 50, 100,
        200, 500, 1000,
        2000, 5000, 10000,
        20000
    };

    juce::Array<float> gains = {
        -24, -12, 0, 12, 24
    };

};
