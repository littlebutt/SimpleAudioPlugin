/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#ifdef __INTELLISENSE__

juce::String JucePlugin_Name = juce::String(); // Mock

#endif

#include "Params.h"

juce::AudioProcessorValueTreeState::ParameterLayout SimpleAudioPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //===============================================================================================================
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::LowCutFreq,
                                                           Params::LowCutFreq,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           20.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::HighCutFreq,
                                                           Params::HighCutFreq,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           20000.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::PeakFreq,
                                                           Params::PeakFreq,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.25f),
                                                           750.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::PeakGain,
                                                           Params::PeakGain,
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
                                                           0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::PeakQuality,
                                                           Params::PeakQuality,
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
                                                           1.f));
    
    juce::StringArray stringArray;
    for( int i = 0; i < 4; ++i )
    {
        juce::String str;
        str << (12 + i*12);
        str << " db/Oct";
        stringArray.add(str);
    }
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::LowCutSlope, Params::LowCutSlope, stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::HighCutSlope, Params::HighCutSlope, stringArray, 0));

    //===============================================================================================================
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ThresholdLowBand,
                                                            Params::ThresholdLowBand,
                                                            juce::NormalisableRange<float>(-60.f, 12.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ThresholdMidBand,
                                                            Params::ThresholdMidBand,
                                                            juce::NormalisableRange<float>(-60.f, 12.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ThresholdHighBand,
                                                            Params::ThresholdHighBand,
                                                            juce::NormalisableRange<float>(-60.f, 12.f, 1.f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::AttackLowBand,
                                                            Params::AttackLowBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 50.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::AttackMidBand,
                                                            Params::AttackMidBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 50.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::AttackHighBand,
                                                            Params::AttackHighBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 50.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ReleaseLowBand,
                                                            Params::ReleaseLowBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 250.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ReleaseMidBand,
                                                            Params::ReleaseMidBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 250.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::ReleaseHighBand,
                                                            Params::ReleaseHighBand,
                                                            juce::NormalisableRange<float>(5.f, 500.f, 1.f, 1.f), 250.f));
    std::vector<float> choices = {1.f, 1.5f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 15.f, 20.f, 50.f, 100.f};
    stringArray.clear();
    for (auto choice : choices)
    {
        stringArray.add(juce::String(choice, 1));
    }
    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::RatioLowBand, Params::RatioLowBand, stringArray, 3));
    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::RatioMidBand, Params::RatioMidBand, stringArray, 3));
    layout.add(std::make_unique<juce::AudioParameterChoice>(Params::RatioHighBand, Params::RatioHighBand, stringArray, 3));

    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::LowMidCrossoverFreq,
                                                            Params::LowMidCrossoverFreq,
                                                            juce::NormalisableRange<float>(20.f, 999.f, 1.f, 1.f), 400));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::MidHighCrossoverFreq,
                                                            Params::MidHighCrossoverFreq,
                                                            juce::NormalisableRange<float>(1000.f, 20000.f, 1.f, 1.f), 2000));
    
    auto gainRange = juce::NormalisableRange<float>(-24.f, 24.f, 1.f, 1.f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::GainIn, Params::GainIn, gainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(Params::GainOut, Params::GainOut, gainRange, 0));
    return layout;
}



void SimpleAudioPluginAudioProcessor::updateFilters()
{
    // update low cut filter
    auto lowCutFreq = apvts.getRawParameterValue(Params::LowCutFreq)->load();
    auto lowCutSlope = Slope(static_cast<int>(apvts.getRawParameterValue(Params::LowCutSlope)->load()));
    auto lowCutFilter = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(lowCutFreq,
                                                                                                    getSampleRate(),
                                                                                                    2 * (lowCutSlope + 1));
    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();
    updateCutFilter(rightLowCut, lowCutFilter, lowCutSlope);
    updateCutFilter(leftLowCut, lowCutFilter, lowCutSlope);

    // update high cut filter
    auto highCutFreq = apvts.getRawParameterValue(Params::HighCutFreq)->load();
    auto highCutSlope = Slope(static_cast<int>(apvts.getRawParameterValue(Params::HighCutSlope)->load()));
    auto highCutFilter = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(highCutFreq,
                                                                                                     getSampleRate(),
                                                                                                     2 * (highCutSlope + 1));
    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    updateCutFilter(rightHighCut, highCutFilter, highCutSlope);
    updateCutFilter(leftHighCut, highCutFilter, highCutSlope);

    // update peak filter
    auto peakFreq = apvts.getRawParameterValue(Params::PeakFreq)->load();
    auto peakQuality = apvts.getRawParameterValue(Params::PeakQuality)->load();
    auto peakGain = apvts.getRawParameterValue(Params::PeakGain)->load();
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(),
                                                               peakFreq,
                                                               peakQuality,
                                                               juce::Decibels::decibelsToGain(peakGain));
    *leftChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;

}

//==============================================================================
SimpleAudioPluginAudioProcessor::SimpleAudioPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
      :
#endif
    apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

SimpleAudioPluginAudioProcessor::~SimpleAudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleAudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleAudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleAudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleAudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleAudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleAudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleAudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleAudioPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleAudioPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleAudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleAudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    
    spec.maximumBlockSize = samplesPerBlock;
    
    spec.numChannels = getTotalNumOutputChannels();
    
    spec.sampleRate = sampleRate;
    
    leftChain.prepare(spec);
    rightChain.prepare(spec);
    
    updateFilters();

    leftChannelFifo.prepare(samplesPerBlock);
    rightChannelFifo.prepare(samplesPerBlock);

    compressorBand.prepare(spec, samplesPerBlock);
}

void SimpleAudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleAudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleAudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateFilters();

    juce::dsp::AudioBlock<float> block(buffer);
    
    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
    //==========================================================================
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressorBand.update(apvts);
    compressorBand.process(buffer);

    //==========================================================================
    leftChannelFifo.update(buffer);
    rightChannelFifo.update(buffer);
}

//==============================================================================
bool SimpleAudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimpleAudioPluginAudioProcessor::createEditor()
{
    return new SimpleAudioPluginAudioProcessorEditor(*this);
}

//==============================================================================
void SimpleAudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void SimpleAudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if( tree.isValid() )
    {
        apvts.replaceState(tree);
        updateFilters();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleAudioPluginAudioProcessor();
}
