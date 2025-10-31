#pragma once

#include <JuceHeader.h>

#include "Fifo.h"

template<typename PathType>
class AnalyzerPathGenerator
{
public:
    void generatePath(const std::vector<float>& renderData,
                    juce::Rectangle<float> fftBounds,
                    int fftSize,
                    float binWidth,
                    float negativeInfinity);

    int getNumPathsAvailable() const;
    bool getPath(PathType& path);

private:
    Fifo<PathType> pathFifo;
};
