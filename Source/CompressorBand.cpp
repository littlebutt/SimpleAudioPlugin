#include "CompressorBand.h"

void CompressorBand::prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock)
{
    compressor.prepare(spec);
    LP.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    HP.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    LP.prepare(spec);
    HP.prepare(spec);
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

    lowCrossover = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(Params::LowMidCrossoverFreq));
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
    auto cutoff = lowCrossover->get();
    LP.setCutoffFrequency(cutoff);
    HP.setCutoffFrequency(cutoff);

    auto fb0Block = juce::dsp::AudioBlock<float>(filterBuffers[0]);
    auto fb1Block = juce::dsp::AudioBlock<float>(filterBuffers[1]);
    auto fb0Ctx = juce::dsp::ProcessContextReplacing<float>(fb0Block);
    auto fb1Ctx = juce::dsp::ProcessContextReplacing<float>(fb1Block);
    LP.process(fb0Ctx);
    HP.process(fb1Ctx);

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
}