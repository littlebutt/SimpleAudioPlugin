#include "ChainSettings.h"

ChainSettings::ChainSettings()
    : peakFreq(0.f), peakGainInDecibels(0.f), peakQuality(1.f),
    lowCutFreq(0.f), highCutFreq(0.f),
    lowCutSlope(Slope_12), highCutSlope(Slope_12)
{
}

ChainSettings::~ChainSettings()
{
}

ChainSettings ChainSettings::getChainSettings(juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;
    
    settings.lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    settings.highCutFreq = apvts.getRawParameterValue("HighCut Freq")->load();
    settings.peakFreq = apvts.getRawParameterValue("Peak Freq")->load();
    settings.peakGainInDecibels = apvts.getRawParameterValue("Peak Gain")->load();
    settings.peakQuality = apvts.getRawParameterValue("Peak Quality")->load();
    settings.lowCutSlope = static_cast<Slope>(apvts.getRawParameterValue("LowCut Slope")->load());
    settings.highCutSlope = static_cast<Slope>(apvts.getRawParameterValue("HighCut Slope")->load());
    
    return settings;
}