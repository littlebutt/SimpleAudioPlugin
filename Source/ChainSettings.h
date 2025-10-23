#pragma once

#include <JuceHeader.h>

enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

class ChainSettings
{
public:
    ChainSettings();
    ~ChainSettings();
    static ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);
    float peakFreq, peakGainInDecibels, peakQuality;
    float lowCutFreq, highCutFreq;
    Slope lowCutSlope, highCutSlope;
};