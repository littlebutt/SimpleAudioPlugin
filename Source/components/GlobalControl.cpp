#include "GlobalControl.h"

GlobalControl::GlobalControl(juce::AudioProcessorValueTreeState& apvts)
: inGainSliderAttachment(apvts, Params::GainIn, inGainSlider),
lowMidCrossoverSliderAttachmennt(apvts, Params::LowMidCrossoverFreq, lowMidCrossoverSlider),
midHighCrossoverSliderAttachment(apvts, Params::MidHighCrossoverFreq, midHighCrossoverSlider),
outGainSliderAttachment(apvts, Params::GainOut, outGainSlider)
{
    addAndMakeVisible(inGainSlider);
    addAndMakeVisible(lowMidCrossoverSlider);
    addAndMakeVisible(midHighCrossoverSlider);
    addAndMakeVisible(outGainSlider);
}

void GlobalControl::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(juce::Colours::blueviolet);
    g.fillAll();

    auto localBounds = bounds;

    bounds.reduce(3, 3);
    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(bounds.toFloat(), 3);

    g.drawRect(localBounds);
}

void GlobalControl::resized()
{
    auto bounds = getLocalBounds();

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