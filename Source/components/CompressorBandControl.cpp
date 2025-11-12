#include "CompressorBandControl.h"

CompressorBandControl::CompressorBandControl(juce::AudioProcessorValueTreeState& apvts)
: attackSlider(*apvts.getParameter(Params::AttackMidBand), "ms"),
releaseSlider(*apvts.getParameter(Params::ReleaseMidBand), "ms"),
thresholdSlider(*apvts.getParameter(Params::ThresholdMidBand), "dB"),
ratioSlider(*apvts.getParameter(Params::RatioMidBand), ""),
attackSliderAttachment(std::make_unique<SliderAttachment>(apvts, Params::AttackMidBand, attackSlider)),
releaseSliderAttachment(std::make_unique<SliderAttachment>(apvts, Params::ReleaseMidBand, releaseSlider)),
thresholdSliderAttachment(std::make_unique<SliderAttachment>(apvts, Params::ThresholdMidBand, thresholdSlider)),
ratioSliderAttachment(std::make_unique<SliderAttachment>(apvts, Params::RatioMidBand, ratioSlider)),
apvts(apvts)
{
    setLookAndFeel(&lnf);
    attackSlider.labels.add({0.f, "5ms"});
    attackSlider.labels.add({1.f, "500ms"});
    releaseSlider.labels.add({0.f, "5ms"});
    releaseSlider.labels.add({1.f, "500ms"});
    thresholdSlider.labels.add({0.f, "-60dB"});
    thresholdSlider.labels.add({1.f, "12dB"});
    ratioSlider.labels.add({0.f, "1"});
    ratioSlider.labels.add({1.f, "100"});
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);

    lowBand.setName("Low");
    midBand.setName("Mid");
    highBand.setName("High");    
    lowBand.setRadioGroupId(1);
    midBand.setRadioGroupId(1);
    highBand.setRadioGroupId(1);

    auto buttonSwitcher = [safePtr = this->safePtr]()
    {
        if (auto* c = safePtr.getComponent())
        {
            c->updateAttachments();
        }
    };

    lowBand.onClick = buttonSwitcher;
    midBand.onClick = buttonSwitcher;
    highBand.onClick = buttonSwitcher;

    lowBand.setToggleState(true, juce::NotificationType::dontSendNotification);

    updateAttachments();

    addAndMakeVisible(lowBand);
    addAndMakeVisible(midBand);
    addAndMakeVisible(highBand);
}

CompressorBandControl::~CompressorBandControl()
{
    setLookAndFeel(nullptr);
}

void CompressorBandControl::paint(juce::Graphics& g)
{

}

void CompressorBandControl::resized()
{
    auto bounds = getLocalBounds().reduced(5);

    using namespace juce;

    FlexBox bandSelectControlBox;
    bandSelectControlBox.flexDirection = FlexBox::Direction::column;
    bandSelectControlBox.flexWrap = FlexBox::Wrap::noWrap;
        
    auto spacer = FlexItem().withHeight(2);

    bandSelectControlBox.items.add(spacer);
    bandSelectControlBox.items.add(FlexItem(lowBand).withFlex(1.f));
    bandSelectControlBox.items.add(spacer);
    bandSelectControlBox.items.add(FlexItem(midBand).withFlex(1.f));
    bandSelectControlBox.items.add(spacer);
    bandSelectControlBox.items.add(FlexItem(highBand).withFlex(1.f));
    bandSelectControlBox.items.add(spacer);

    FlexBox flexBox;
    flexBox.flexWrap = FlexBox::Wrap::noWrap;
    flexBox.flexDirection = FlexBox::Direction::row;
    flexBox.items.add(FlexItem(bandSelectControlBox).withWidth(50));
    flexBox.items.add(FlexItem(attackSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(releaseSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(thresholdSlider).withFlex(1.f));
    flexBox.items.add(FlexItem(ratioSlider).withFlex(1.f));
    flexBox.performLayout(bounds);
}

void CompressorBandControl::updateAttachments()
{
    BandType bandType = [this]()
    {
        if (lowBand.getToggleState())
        {
            return Low;
        }
        else if (midBand.getToggleState())
        {
            return Mid;
        }
        else
        {
            return High;
        }
    }();

    attackSliderAttachment.reset();
    releaseSliderAttachment.reset();
    thresholdSliderAttachment.reset();
    ratioSliderAttachment.reset();
    switch (bandType)
    {
        case Low:
        {
            attackSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::AttackLowBand, attackSlider);
            releaseSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ReleaseLowBand, releaseSlider);
            thresholdSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ThresholdLowBand, thresholdSlider);
            ratioSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::RatioLowBand, ratioSlider);
            break;
        }
        case Mid:
        {
            attackSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::AttackMidBand, attackSlider);
            releaseSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ReleaseMidBand, releaseSlider);
            thresholdSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ThresholdMidBand, thresholdSlider);
            ratioSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::RatioMidBand, ratioSlider);
            break;
        }
        case High:
        {
            attackSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::AttackHighBand, attackSlider);
            releaseSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ReleaseHighBand, releaseSlider);
            thresholdSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::ThresholdHighBand, thresholdSlider);
            ratioSliderAttachment = std::make_unique<SliderAttachment>(apvts, Params::RatioHighBand, ratioSlider);
            break;
        }
    }
}