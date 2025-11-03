#include "CompressorBand.h"

void CompressorBand::prepare(juce::dsp::ProcessSpec& spec)
{
    compressor.prepare(spec);
}

void CompressorBand::update(juce::AudioProcessorValueTreeState& apvts)
{
    auto attack = apvts.getRawParameterValue(Params::AttackLowBand)->load();
    compressor.setAttack(attack);
    auto threshold = apvts.getRawParameterValue(Params::ThresholdLowBand)->load();
    compressor.setThreshold(threshold);
    auto release = apvts.getRawParameterValue(Params::ReleaseLowBand)->load();
    compressor.setRelease(release);
    auto ratio = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(Params::RatioLowBand));
    compressor.setRatio(ratio->getCurrentChoiceName().getFloatValue());
}


void CompressorBand::process(juce::dsp::ProcessContextReplacing<float> context)
{
    compressor.process(context);
}