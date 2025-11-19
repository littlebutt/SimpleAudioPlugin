#include "LookAndFeel.h"
#include "RotarySliderWithLabels.h"

void LookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                    float sliderPosProportional, float rotaryStartAngle,
                                    float rotaryEndAngle, juce::Slider& slider)
{
    using namespace juce;

    auto bounds = Rectangle<float>(x, y, width, height);
    auto enabled = slider.isEnabled();

    g.setColour(enabled ? Colour(97u, 18u, 167u) : Colours::darkgrey);
    g.fillEllipse(bounds);

    g.setColour(enabled ? Colour(255u, 154u, 1u) : Colours::grey);
    g.drawEllipse(bounds, 1.f);

    if (auto* rswl = dynamic_cast<RotarySliderWithLabels*>(&slider))
    {
        auto center = bounds.getCentre();
        Path p;
        Rectangle<float> r;
        r.setLeft(center.getX() - 2);
        r.setRight(center.getX() + 2);
        r.setTop(bounds.getY());
        r.setBottom(center.getY() - rswl->getTextHeight() * 1.5);
        p.addRoundedRectangle(r, 2.f);

        auto sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
        p.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));

        g.fillPath(p);

        g.setFont(rswl->getTextHeight());
        auto text = rswl->getDisplayString();
        GlyphArrangement arr;
        arr.addLineOfText(g.getCurrentFont(), text, 0, 0);
        auto strWidth = arr.getBoundingBox(0, -1, true).getWidth();
        
        r.setSize(strWidth + 4, rswl->getTextHeight() + 2);
        r.setCentre(bounds.getCentre());
        
        g.setColour(enabled ? Colours::black : Colours::darkgrey);
        g.fillRect(r);
        
        g.setColour(enabled ? Colours::white : Colours::lightgrey);
        g.drawFittedText(text, r.toNearestInt(), Justification::centred, 1);
    }
}

void LookAndFeel::drawToggleButton(juce::Graphics &g,
                                    juce::ToggleButton &toggleButton,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown)
{
    auto bounds = toggleButton.getLocalBounds().reduced(2);        
    auto buttonIsOn = toggleButton.getToggleState();
    const int cornerSize = 4;
    g.setColour(buttonIsOn ? juce::Colours::white : juce::Colours::black );
    g.fillRoundedRectangle(bounds.toFloat(), cornerSize);
    g.setColour(buttonIsOn ? juce::Colours::black : juce::Colours::white );
    g.drawRoundedRectangle(bounds.toFloat(), cornerSize, 1);
    g.drawFittedText(toggleButton.getName(), bounds, juce::Justification::centred, 1);
}