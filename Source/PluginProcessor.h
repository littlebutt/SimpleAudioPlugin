/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginEditor.h"

#include "CompressorBand.h"

enum Slope
{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

enum ChainPositions
{
    LowCut,
    Peak,
    HighCut
};

/*
    Update the high cut or low cut filter frequency coefficients by changing slopes with index.

    @param chain           The target CutFilter, HighCut or LowCut
    @param coefficients    Coefficients of the CutFilter to set
*/
template<int Index, typename ChainType, typename CoefficientType>
void update(ChainType& chain, const CoefficientType& coefficients)
{
    *chain.template get<Index>().coefficients = *coefficients[Index];
    chain.template setBypassed<Index>(false);
}

/*
    Update the high cut or low cut filter frequency coefficients and change slopes.

    @param chain           The target CutFilter, HighCut or LowCut
    @param coefficients    Coefficients of the CutFilter to set
    @param slope           The Slope to set, Slope_12, Slope_24, Slope_32 or Slope_48
*/
template<typename ChainType, typename CoefficientType>
void updateCutFilter(ChainType& chain,
                    const CoefficientType& coefficients,
                    const Slope& slope)
{
    chain.template setBypassed<0>(true);
    chain.template setBypassed<1>(true);
    chain.template setBypassed<2>(true);
    chain.template setBypassed<3>(true);
    
    switch(slope)
    {
        case Slope_48:
        {
            update<3>(chain, coefficients); [[fallthrough]];
        }
        case Slope_36:
        {
            update<2>(chain, coefficients); [[fallthrough]];
        }
        case Slope_24:
        {
            update<1>(chain, coefficients); [[fallthrough]];
        }
        case Slope_12:
        {
            update<0>(chain, coefficients);
        }
    }
}

//==============================================================================
/**
*/
class SimpleAudioPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleAudioPluginAudioProcessor();
    ~SimpleAudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void updateFilters();

    //==============================================================================
    // AudioProcessorValueTreeState for the plugin processor
    juce::AudioProcessorValueTreeState apvts;

    SingleChannelSampleFifo<juce::AudioBuffer<float>> leftChannelFifo { Channel::Left };
    SingleChannelSampleFifo<juce::AudioBuffer<float>> rightChannelFifo { Channel::Right };

private:
    // ProcessorChain for processing left channel and right channel
    MonoChain leftChain, rightChain;

    //==============================================================================
    // Compressor
    CompressorBand compressorBand;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleAudioPluginAudioProcessor)
};
