#pragma once

#include <JuceHeader.h>

namespace Params
{
    const juce::String LowCutFreq {"LowCut Freq"};
    const juce::String HighCutFreq {"HighCut Freq"};
    const juce::String PeakFreq {"Peak Freq"};
    const juce::String PeakGain {"Peak Gain"};
    const juce::String PeakQuality {"Peak Quality"};
    const juce::String LowCutSlope {"LowCut Slope"};
    const juce::String HighCutSlope {"HighCut Slope"};

    const juce::String LowMidCrossoverFreq {"Low Mid Crossover Freq"};
    const juce::String MidHighCrossoverFreq {"Mid High Crossover Freq"};
    const juce::String ThresholdLowBand {"Threshold Low Band"};
    const juce::String ThresholdMidBand {"Threshold Mid Band"};
    const juce::String ThresholdHighBand {"Threshold High Band"};
    const juce::String AttackLowBand {"Attack Low Band"};
    const juce::String AttackMidBand {"Attack Mid Band"};
    const juce::String AttackHighBand {"Attack High Band"};
    const juce::String ReleaseLowBand {"Release Low Band"};
    const juce::String ReleaseMidBand {"Release Mid Band"};
    const juce::String ReleaseHighBand {"Release High Band"};
    const juce::String RatioLowBand {"RatioLowBand"};
    const juce::String RatioMidBand {"RatioMidBand"};
    const juce::String RatioHighBand {"RatioHighBand"};
    const juce::String GainIn {"Gain In"};
    const juce::String GainOut {"Gain Out"};
}