#include "GlobalControl.h"

GlobalControl::GlobalControl(juce::AudioProcessorValueTreeState& apvts)
: inGainSlider(*apvts.getParameter(Params::GainIn), "dB"),
lowMidCrossoverSlider(*apvts.getParameter(Params::LowMidCrossoverFreq), "Hz"),
midHighCrossoverSlider(*apvts.getParameter(Params::MidHighCrossoverFreq), "Hz"),
outGainSlider(*apvts.getParameter(Params::GainOut), "dB"),
inGainSliderAttachment(apvts, Params::GainIn, inGainSlider),
lowMidCrossoverSliderAttachmennt(apvts, Params::LowMidCrossoverFreq, lowMidCrossoverSlider),
midHighCrossoverSliderAttachment(apvts, Params::MidHighCrossoverFreq, midHighCrossoverSlider),
outGainSliderAttachment(apvts, Params::GainOut, outGainSlider)
{
    inGainSlider.labels.add({0.f, "-24dB"});
    inGainSlider.labels.add({1.f, "24dB"});
    lowMidCrossoverSlider.labels.add({0.f, "20Hz"});
    lowMidCrossoverSlider.labels.add({1.f, "999Hz"});
    midHighCrossoverSlider.labels.add({0.f, "1kHz"});
    midHighCrossoverSlider.labels.add({1.f, "20kHz"});
    outGainSlider.labels.add({0.f, "-24dB"});
    outGainSlider.labels.add({1.f, "24dB"});
    addAndMakeVisible(inGainSlider);
    addAndMakeVisible(lowMidCrossoverSlider);
    addAndMakeVisible(midHighCrossoverSlider);
    addAndMakeVisible(outGainSlider);
}

void GlobalControl::paint(juce::Graphics& g)
{
   
}

void GlobalControl::resized()
{
    auto bounds = getLocalBounds().reduced(5);

    using namespace juce;

    FlexBox flexBox;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.items.add(FlexItem(inGainSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(lowMidCrossoverSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(midHighCrossoverSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(outGainSlider).withFlex(1.f));
    flexBox.performLayout(bounds);
}