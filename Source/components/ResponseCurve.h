#pragma once

#include <JuceHeader.h>
#include "../PathProducer.h"

class SimpleAudioPluginAudioProcessor;

using Filter = juce::dsp::IIR::Filter<float>;

using CutFilter = juce::dsp::ProcessorChain<Filter /* Slope 12 */, Filter /* Slope 24 */, Filter /* Slope 36 */, Filter /* Slope 48 */>;

using MonoChain = juce::dsp::ProcessorChain<CutFilter /* LowCut */, Filter /* Peak */, CutFilter /* HighCut */>;

class ResponseCurve : public juce::Component,
                        juce::AudioProcessorParameter::Listener,
                        juce::Timer
{
public:
    ResponseCurve(SimpleAudioPluginAudioProcessor&);
    ~ResponseCurve();

    //=============================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    //=============================================================
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

    //=============================================================
    void timerCallback();
private:
    SimpleAudioPluginAudioProcessor& audioProcessor;

    juce::Rectangle<int> getRenderArea();
    juce::Rectangle<int> getAnalysisArea();

    //=============================================================
    juce::Atomic<bool> parametersChanged { false };

    MonoChain monoChain;

    void updateResponseCurve();
    
    juce::Path responseCurve;

    void updateChain();

    PathProducer leftPathProducer, rightPathProducer;

    //=============================================================
    juce::Array<float> freqs = {
        20, 50, 100,
        200, 500, 1000,
        2000, 5000, 10000,
        20000
    };

    juce::Array<float> gains = {
        -24, -12, 0, 12, 24
    };

};
