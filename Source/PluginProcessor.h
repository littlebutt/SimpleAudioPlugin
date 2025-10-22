/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ChainSettings.h"

using Filter = juce::dsp::IIR::Filter<float>;

using CutFilter = juce::dsp::ProcessorChain<Filter /* Slope 12 */, Filter /* Slope 24 */, Filter /* Slope 36 */, Filter /* Slope 48 */>;

using MonoChain = juce::dsp::ProcessorChain<CutFilter /* LowCut */, Filter /* Peak */, CutFilter /* HighCut */>;


enum ChainPositions
{
    LowCut,
    Peak,
    HighCut
};

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

private:

    // AudioProcessorValueTreeState for the plugin processor
    juce::AudioProcessorValueTreeState apvts;

    // ProcessorChain for processing left channel and right channel
    MonoChain leftChain, rightChain;

    /*
    Update the high cut or low cut filter frequency coefficients by changing slopes with index.

    @param chain           The target CutFilter, HighCut or LowCut
    @param coefficients    Coefficients of the CutFilter to set
    */
    template<int Index, typename ChainType, typename CoefficientType>
    void update(ChainType& chain, const CoefficientType& coefficients);

    /*
    Update the high cut or low cut filter frequency coefficients and change slopes.

    @param chain           The target CutFilter, HighCut or LowCut
    @param coefficients    Coefficients of the CutFilter to set
    @param slope           The Slope to set, Slope_12, Slope_24, Slope_32 or Slope_48
    */
    template<typename ChainType, typename CoefficientType>
    void updateCutFilter(ChainType& chain,
                         const CoefficientType& coefficients,
                         const Slope& slope);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleAudioPluginAudioProcessor)
};
