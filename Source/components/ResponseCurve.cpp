#include "ResponseCurve.h"
#include "../PluginProcessor.h"

ResponseCurve::ResponseCurve(SimpleAudioPluginAudioProcessor& p)
: audioProcessor(p),
leftPathProducer(audioProcessor.leftChannelFifo),
rightPathProducer(audioProcessor.rightChannelFifo)
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->addListener(this);
    }

    updateChain();
    
    startTimerHz(60);
}

ResponseCurve::~ResponseCurve()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params)
    {
        param->removeListener(this);
    }
}

void ResponseCurve::paint(juce::Graphics& g)
{
    using namespace juce;

    g.fillAll(Colours::black);
    
    auto renderArea = getAnalysisArea();
    auto left = renderArea.getX();
    auto right = renderArea.getRight();
    auto top = renderArea.getY();
    auto bottom = renderArea.getBottom();
    auto width = renderArea.getWidth();
    
    g.setColour(Colours::dimgrey);

    for (auto x : freqs)
    {
        auto normX = mapFromLog10(x, 20.f, 20000.f);
        g.drawVerticalLine(left + width * normX, top, bottom);
    }

    for (auto gDb : gains)
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        
        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u) : Colours::darkgrey );
        g.drawHorizontalLine(y, left, right);
    }

    // Draw curve
    auto leftChannelFFTPath = leftPathProducer.getPath();
    leftChannelFFTPath.applyTransform(AffineTransform().translation(renderArea.getX(), renderArea.getY()));
    
    g.setColour(Colour(97u, 18u, 167u)); //purple-
    g.strokePath(leftChannelFFTPath, PathStrokeType(1.f));
    
    auto rightChannelFFTPath = rightPathProducer.getPath();
    rightChannelFFTPath.applyTransform(AffineTransform().translation(renderArea.getX(), renderArea.getY()));
    
    g.setColour(Colour(215u, 201u, 134u));
    g.strokePath(rightChannelFFTPath, PathStrokeType(1.f));

    g.setColour(Colours::white);
    g.strokePath(responseCurve, PathStrokeType(2.f));
    
    Path border;
    
    border.setUsingNonZeroWinding(false);
    
    border.addRoundedRectangle(getRenderArea(), 4);
    border.addRectangle(getLocalBounds());
    
    g.setColour(Colours::black);
    
    g.fillPath(border);
    
    // Draw text labels
    g.setColour(Colours::lightgrey);
    const int fontHeight = 10;
    g.setFont(fontHeight);

    
    for (auto x : freqs)
    {
        auto normX = mapFromLog10(x, 20.f, 20000.f);

        bool addK = false;
        String str;
        if( x > 999.f )
        {
            addK = true;
            x /= 1000.f;
        }

        str << x;
        if( addK )
            str << "k";
        str << "Hz";
        
        GlyphArrangement arr;
        arr.addLineOfText(g.getCurrentFont(), str, 0, 0);
        auto textWidth = arr.getBoundingBox(0, -1, true).getWidth();

        Rectangle<int> r;

        r.setSize(textWidth, fontHeight);
        r.setCentre(left + width * normX, 0);
        r.setY(1);
        
        g.drawFittedText(str, r, juce::Justification::centred, 1);
    }

    for( auto gDb : gains )
    {
        auto y = jmap(gDb, -24.f, 24.f, float(bottom), float(top));
        
        String str;
        if( gDb > 0 )
            str << "+";
        str << gDb;
        
        GlyphArrangement arr;
        arr.addLineOfText(g.getCurrentFont(), str, 0, 0);
        auto textWidth = arr.getBoundingBox(0, -1, true).getWidth();
        
        Rectangle<int> r;
        r.setSize(textWidth, fontHeight);
        r.setX(getWidth() - textWidth);
        r.setCentre(r.getCentreX(), y);
        
        g.setColour(gDb == 0.f ? Colour(0u, 172u, 1u) : Colours::lightgrey );
        
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
        
        str.clear();
        str << (gDb - 24.f);

        r.setX(1);
        arr.clear();
        arr.addLineOfText(g.getCurrentFont(), str, 0, 0);
        textWidth = arr.getBoundingBox(0, -1, true).getWidth();
        r.setSize(textWidth, fontHeight);
        g.setColour(Colours::lightgrey);
        g.drawFittedText(str, r, juce::Justification::centredLeft, 1);
    }
    
    g.setColour(Colours::orange);
    g.drawRoundedRectangle(getRenderArea().toFloat(), 4.f, 1.f);
    
}

void ResponseCurve::resized()
{
    using namespace juce;
    
    responseCurve.preallocateSpace(getWidth() * 3);
    updateResponseCurve();
}

void ResponseCurve::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

void ResponseCurve::parameterGestureChanged(int parameterIndex, bool gestureIsStarting) { }

void ResponseCurve::timerCallback()
{
    auto fftBounds = getAnalysisArea().toFloat();
    auto sampleRate = audioProcessor.getSampleRate();
        
    leftPathProducer.process(fftBounds, sampleRate);
    rightPathProducer.process(fftBounds, sampleRate);

    if (parametersChanged.compareAndSetBool(false, true))
    {
        updateChain();
        updateResponseCurve();
    }
    
    repaint();
}

void ResponseCurve::updateResponseCurve()
{
    using namespace juce;
    auto responseArea = getAnalysisArea();
    
    auto w = responseArea.getWidth();
    
    auto& lowcut = monoChain.get<ChainPositions::LowCut>();
    auto& peak = monoChain.get<ChainPositions::Peak>();
    auto& highcut = monoChain.get<ChainPositions::HighCut>();
    
    auto sampleRate = audioProcessor.getSampleRate();
    
    std::vector<double> mags;
    
    mags.resize(w);
    
    for( int i = 0; i < w; ++i )
    {
        double mag = 1.f;
        auto freq = mapToLog10(double(i) / double(w), 20.0, 20000.0);
        
        if(! monoChain.isBypassed<ChainPositions::Peak>() )
            mag *= peak.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        
        if( !monoChain.isBypassed<ChainPositions::LowCut>() )
        {
            if( !lowcut.isBypassed<0>() )
                mag *= lowcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !lowcut.isBypassed<1>() )
                mag *= lowcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !lowcut.isBypassed<2>() )
                mag *= lowcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !lowcut.isBypassed<3>() )
                mag *= lowcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
        
        if( !monoChain.isBypassed<ChainPositions::HighCut>() )
        {
            if( !highcut.isBypassed<0>() )
                mag *= highcut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !highcut.isBypassed<1>() )
                mag *= highcut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !highcut.isBypassed<2>() )
                mag *= highcut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if( !highcut.isBypassed<3>() )
                mag *= highcut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
        }
            
        mags[i] = Decibels::gainToDecibels(mag);
    }
    
    responseCurve.clear();
    
    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input)
    {
        return jmap(input, -24.0, 24.0, outputMin, outputMax);
    };
    responseCurve.startNewSubPath(responseArea.getX(), map(mags.front()));
    for( size_t i = 1; i < mags.size(); ++i )
    {
        responseCurve.lineTo(responseArea.getX() + i, map(mags[i]));
    }
}

void ResponseCurve::updateChain()
{
    // update low cut filter
    auto lowCutFreq = audioProcessor.apvts.getRawParameterValue("LowCut Freq")->load();
    auto lowCutSlope = Slope(static_cast<int>(audioProcessor.apvts.getRawParameterValue("LowCut Slope")->load()));
    auto lowCutFilter = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(lowCutFreq,
                                                                                                    audioProcessor.getSampleRate(),
                                                                                                    2 * (lowCutSlope + 1));
    auto& leftLowCut = monoChain.get<ChainPositions::LowCut>();
    updateCutFilter(leftLowCut, lowCutFilter, lowCutSlope);

    // update high cut filter
    auto highCutFreq = audioProcessor.apvts.getRawParameterValue("HighCut Freq")->load();
    auto highCutSlope = Slope(static_cast<int>(audioProcessor.apvts.getRawParameterValue("HighCut Slope")->load()));
    auto highCutFilter = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(highCutFreq,
                                                                                                     audioProcessor.getSampleRate(),
                                                                                                     2 * (highCutSlope + 1));
    auto& leftHighCut = monoChain.get<ChainPositions::HighCut>();
    updateCutFilter(leftHighCut, highCutFilter, highCutSlope);

    // update peak filter
    auto peakFreq = audioProcessor.apvts.getRawParameterValue("Peak Freq")->load();
    auto peakQuality = audioProcessor.apvts.getRawParameterValue("Peak Quality")->load();
    auto peakGain = audioProcessor.apvts.getRawParameterValue("Peak Gain")->load();
    auto peakCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(audioProcessor.getSampleRate(),
                                                               peakFreq,
                                                               peakQuality,
                                                               juce::Decibels::decibelsToGain(peakGain));
    *monoChain.get<ChainPositions::Peak>().coefficients = *peakCoefficients;

}

juce::Rectangle<int> ResponseCurve::getRenderArea()
{
    auto bounds = getLocalBounds();
    
    bounds.removeFromTop(12);
    bounds.removeFromBottom(2);
    bounds.removeFromLeft(20);
    bounds.removeFromRight(20);
    
    return bounds;
}

juce::Rectangle<int> ResponseCurve::getAnalysisArea()
{
    auto bounds = getRenderArea();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    return bounds;
}