#include "CompressorBand.h"

void CompressorBand::prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock)
{
    compressor.prepare(spec);
    LP1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    HP1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    AP2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    LP2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    HP2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    LP1.prepare(spec);
    HP1.prepare(spec);
    AP2.prepare(spec);
    LP2.prepare(spec);
    HP2.prepare(spec);
    for (auto& buffer : filterBuffers)
    {
        buffer.setSize(spec.numChannels, samplesPerBlock);
    }
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

    lowMidCrossover = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(Params::LowMidCrossoverFreq));
    midHighCrossover = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(Params::MidHighCrossoverFreq));
}


void CompressorBand::process(juce::dsp::ProcessContextReplacing<float> context)
{
    compressor.process(context);
}

void CompressorBand::process(juce::AudioSampleBuffer& buffer)
{
    for (auto& fb : filterBuffers)
    {
        fb = buffer;
    }
    auto lowMidCutoff = lowMidCrossover->get();
    LP1.setCutoffFrequency(lowMidCutoff);
    HP1.setCutoffFrequency(lowMidCutoff);

    auto midHighCutoff = midHighCrossover->get();
    AP2.setCutoffFrequency(midHighCutoff);
    LP2.setCutoffFrequency(midHighCutoff);
    HP2.setCutoffFrequency(midHighCutoff);

    auto fb0Block = juce::dsp::AudioBlock<float>(filterBuffers[0]);
    auto fb1Block = juce::dsp::AudioBlock<float>(filterBuffers[1]);
    auto fb2Block = juce::dsp::AudioBlock<float>(filterBuffers[2]);
    auto fb0Ctx = juce::dsp::ProcessContextReplacing<float>(fb0Block);
    auto fb1Ctx = juce::dsp::ProcessContextReplacing<float>(fb1Block);
    auto fb2Ctx = juce::dsp::ProcessContextReplacing<float>(fb2Block);
    LP1.process(fb0Ctx);
    AP2.process(fb0Ctx);

    HP1.process(fb1Ctx);
    filterBuffers[2] = filterBuffers[1];
    LP2.process(fb1Ctx);
    HP2.process(fb2Ctx);

    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    buffer.clear();

    auto addFilterBand = [nc = numChannels, ns = numSamples](auto& inputBuffer, const auto& source)
    {
        for (int i = 0; i < nc; i++)
        {
            inputBuffer.addFrom(i, 0, source, i, 0, ns);
        }
    };

    addFilterBand(buffer, filterBuffers[0]);
    addFilterBand(buffer, filterBuffers[1]);
    addFilterBand(buffer, filterBuffers[2]);
}