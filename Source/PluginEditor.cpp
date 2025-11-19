/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params.h"

//==============================================================================
SimpleAudioPluginAudioProcessorEditor::SimpleAudioPluginAudioProcessorEditor (SimpleAudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    peakFreqSlider(*audioProcessor.apvts.getParameter(Params::PeakFreq), "Hz"),
    peakGainSlider(*audioProcessor.apvts.getParameter(Params::PeakGain), "dB"),
    peakQualitySlider(*audioProcessor.apvts.getParameter(Params::PeakQuality), ""),
    lowCutFreqSlider(*audioProcessor.apvts.getParameter(Params::LowCutFreq), "Hz"),
    highCutFreqSlider(*audioProcessor.apvts.getParameter(Params::HighCutFreq), "Hz"),
    lowCutSlopeSlider(*audioProcessor.apvts.getParameter(Params::LowCutSlope), "dB/Oct"),
    highCutSlopeSlider(*audioProcessor.apvts.getParameter(Params::HighCutSlope), "dB/Oct"),

    responseCurve(audioProcessor),

    peakFreqSliderAttachment(audioProcessor.apvts, Params::PeakFreq, peakFreqSlider),
    peakGainSliderAttachment(audioProcessor.apvts, Params::PeakGain, peakGainSlider),
    peakQualitySliderAttachment(audioProcessor.apvts, Params::PeakQuality, peakQualitySlider),
    lowCutFreqSliderAttachment(audioProcessor.apvts, Params::LowCutFreq, lowCutFreqSlider),
    highCutFreqSliderAttachment(audioProcessor.apvts, Params::HighCutFreq, highCutFreqSlider),
    lowCutSlopeSliderAttachment(audioProcessor.apvts, Params::LowCutSlope, lowCutSlopeSlider),
    highCutSlopeSliderAttachment(audioProcessor.apvts, Params::HighCutSlope, highCutSlopeSlider),
    globalControl(audioProcessor.apvts),
    compressorBandControl(audioProcessor.apvts)
{
    peakFreqSlider.labels.add({0.f, "20Hz"});
    peakFreqSlider.labels.add({1.f, "20kHz"});
    
    peakGainSlider.labels.add({0.f, "-24dB"});
    peakGainSlider.labels.add({1.f, "+24dB"});
    
    peakQualitySlider.labels.add({0.f, "0.1"});
    peakQualitySlider.labels.add({1.f, "10.0"});
    
    lowCutFreqSlider.labels.add({0.f, "20Hz"});
    lowCutFreqSlider.labels.add({1.f, "20kHz"});
    
    highCutFreqSlider.labels.add({0.f, "20Hz"});
    highCutFreqSlider.labels.add({1.f, "20kHz"});
    
    lowCutSlopeSlider.labels.add({0.0f, "12"});
    lowCutSlopeSlider.labels.add({1.f, "48"});
    
    highCutSlopeSlider.labels.add({0.0f, "12"});
    highCutSlopeSlider.labels.add({1.f, "48"});

    for( auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }

    addAndMakeVisible(globalControl);
    addAndMakeVisible(compressorBandControl);
    setSize(600, 800);
}

SimpleAudioPluginAudioProcessorEditor::~SimpleAudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void SimpleAudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    using namespace juce;
    g.fillAll(Colours::black);

    Path curve;

    auto bounds = getLocalBounds();
    auto center = bounds.getCentre();

    g.setFont(Font(FontOptions("Noto Sans", 16.0f, Font::plain)));
    String title {"Simple Audio Plugin"};
    g.setFont(30);
    GlyphArrangement arr;
    arr.addLineOfText(g.getCurrentFont(), title, 0, 0);
    auto titleWidth = arr.getBoundingBox(0, -1, true).getWidth();

    curve.startNewSubPath(center.x, 32);
    curve.lineTo(center.x - titleWidth * 0.45f, 32);
    
    auto cornerSize = 20;
    auto curvePos = curve.getCurrentPosition();
    curve.quadraticTo(curvePos.getX() - cornerSize, curvePos.getY(),
                      curvePos.getX() - cornerSize, curvePos.getY() - 16);
    curvePos = curve.getCurrentPosition();
    curve.quadraticTo(curvePos.getX(), 2,
                      curvePos.getX() - cornerSize, 2);
    
    curve.lineTo(0.f, 2.f);
    curve.lineTo(0.f, 0.f);
    curve.lineTo(center.x, 0.f);
    curve.closeSubPath();

    g.setColour(Colour(97u, 18u, 167u));
    g.fillPath(curve);
    
    curve.applyTransform(AffineTransform().scaled(-1, 1));
    curve.applyTransform(AffineTransform().translated(getWidth(), 0));
    g.fillPath(curve);

    g.setColour(Colour(255u, 154u, 1u));
    g.drawFittedText(title, bounds, juce::Justification::centredTop, 1);
    
    g.setColour(Colours::grey);
    g.setFont(14);
    g.drawFittedText("LowCut", lowCutSlopeSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("Peak", peakQualitySlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("HighCut", highCutSlopeSlider.getBounds(), juce::Justification::centredBottom, 1);
    g.drawFittedText("GainIn", globalControl.getBounds().withX(60), juce::Justification::bottomLeft, 1);
    g.drawFittedText("LowMidFreq", globalControl.getBounds().withX(210), juce::Justification::bottomLeft, 1);
    g.drawFittedText("MidHighFreq", globalControl.getBounds().withX(350), juce::Justification::bottomLeft, 1);
    g.drawFittedText("GainOut", globalControl.getBounds().withX(500), juce::Justification::bottomLeft, 1);
    g.drawFittedText("Attack", compressorBandControl.getBounds().withX(100), juce::Justification::bottomLeft, 1);
    g.drawFittedText("Release", compressorBandControl.getBounds().withX(240), juce::Justification::bottomLeft, 1);
    g.drawFittedText("Threshold", compressorBandControl.getBounds().withX(370), juce::Justification::bottomLeft, 1);
    g.drawFittedText("Ratio", compressorBandControl.getBounds().withX(520), juce::Justification::bottomLeft, 1);
    
    auto buildDate = Time::getCompilationDate().toString(true, false);
    auto buildTime = Time::getCompilationDate().toString(false, true);
    g.setFont(12);
    g.drawFittedText("Build: " + buildDate + "\n" + buildTime, highCutSlopeSlider.getBounds().withY(6), Justification::topRight, 2);
    
}

void SimpleAudioPluginAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(4);
    bounds.removeFromTop(25);
    bounds.removeFromTop(5);

    float hRatio = 25.f / 100.f;
    auto responseArea = bounds.removeFromTop(bounds.getHeight() * hRatio);

    responseCurve.setBounds(responseArea);

    bounds.removeFromTop(5);

    auto eqBounds = bounds.removeFromTop(300); 
    auto lowCutArea = eqBounds.removeFromLeft(eqBounds.getWidth() * 0.33);
    auto highCutArea = eqBounds.removeFromRight(eqBounds.getWidth() * 0.5);
    lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight() * 0.5));
    lowCutSlopeSlider.setBounds(lowCutArea);

    highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight() * 0.5));
    highCutSlopeSlider.setBounds(highCutArea);

    peakFreqSlider.setBounds(eqBounds.removeFromTop(eqBounds.getHeight() * 0.33));
    peakGainSlider.setBounds(eqBounds.removeFromTop(eqBounds.getHeight() * 0.5));
    peakQualitySlider.setBounds(eqBounds.removeFromTop(eqBounds.getHeight()));
    globalControl.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.5));
    compressorBandControl.setBounds(bounds);
}

juce::Array<juce::Component*> SimpleAudioPluginAudioProcessorEditor::getComps()
{
    juce::Array<juce::Component*> array {
                                            &peakFreqSlider,
                                            &peakGainSlider,
                                            &peakQualitySlider,
                                            &lowCutFreqSlider,
                                            &highCutFreqSlider,
                                            &lowCutSlopeSlider,
                                            &highCutSlopeSlider,

                                            &responseCurve
                                        };
    return array;
}
