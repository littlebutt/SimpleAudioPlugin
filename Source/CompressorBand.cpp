#include "CompressorBand.h"

void CompressorBand::prepare(juce::dsp::ProcessSpec& spec, int samplesPerBlock)
{
    compressors[0].prepare(spec);
    compressors[1].prepare(spec);
    compressors[2].prepare(spec);
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
    auto attackLow = apvts.getRawParameterValue(Params::AttackLowBand)->load();
    compressors[0].setAttack(attackLow);
    auto attackMid = apvts.getRawParameterValue(Params::AttackMidBand)->load();
    compressors[1].setAttack(attackMid);
    auto attackHigh = apvts.getRawParameterValue(Params::AttackHighBand)->load();
    compressors[2].setAttack(attackHigh);
    auto thresholdLow = apvts.getRawParameterValue(Params::ThresholdLowBand)->load();
    compressors[0].setThreshold(thresholdLow);
    auto thresholdMid = apvts.getRawParameterValue(Params::ThresholdMidBand)->load();
    compressors[1].setThreshold(thresholdMid);
    auto thresholdHigh = apvts.getRawParameterValue(Params::ThresholdHighBand)->load();
    compressors[2].setThreshold(thresholdHigh);
    auto releaseLow = apvts.getRawParameterValue(Params::ReleaseLowBand)->load();
    compressors[0].setRelease(releaseLow);
    auto releaseMid = apvts.getRawParameterValue(Params::ReleaseMidBand)->load();
    compressors[1].setRelease(releaseMid);
    auto releaseHigh = apvts.getRawParameterValue(Params::ReleaseHighBand)->load();
    compressors[2].setRelease(releaseHigh);
    auto ratioLow = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(Params::RatioLowBand));
    compressors[0].setRatio(ratioLow->getCurrentChoiceName().getFloatValue());
    auto ratioMid = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(Params::RatioMidBand));
    compressors[1].setRatio(ratioMid->getCurrentChoiceName().getFloatValue());
    auto ratioHigh = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(Params::RatioHighBand));
    compressors[2].setRatio(ratioHigh->getCurrentChoiceName().getFloatValue());

    lowMidCrossover = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(Params::LowMidCrossoverFreq));
    midHighCrossover = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(Params::MidHighCrossoverFreq));
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

    for( size_t i = 0; i < filterBuffers.size(); ++i )
    {
        auto block = juce::dsp::AudioBlock<float>(filterBuffers[i]);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        compressors[i].process(context);
    }
    
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