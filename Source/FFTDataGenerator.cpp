#include "FFTDataGenerator.h"

template<typename BlockType>
void FFTDataGenerator<BlockType>::produceFFTDataForRendering(const juce::AudioBuffer<float>& audioData, const float negativeInfinity)
{
    const auto fftSize = getFFTSize();
        
        fftData.assign(fftData.size(), 0);
        auto* readIndex = audioData.getReadPointer(0);
        std::copy(readIndex, readIndex + fftSize, fftData.begin());
        
        // first apply a windowing function to our data
        window->multiplyWithWindowingTable(fftData.data(), fftSize);
        
        // then render our FFT data..
        forwardFFT->performFrequencyOnlyForwardTransform(fftData.data());
        
        int numBins = (int)fftSize / 2;
        
        //normalize the fft values.
        for( int i = 0; i < numBins; ++i )
        {
            auto v = fftData[i];
            if( !std::isinf(v) && !std::isnan(v) )
            {
                v /= float(numBins);
            }
            else
            {
                v = 0.f;
            }
            fftData[i] = v;
        }
        
        //convert them to decibels
        for( int i = 0; i < numBins; ++i )
        {
            fftData[i] = juce::Decibels::gainToDecibels(fftData[i], negativeInfinity);
        }
        
        fftDataFifo.push(fftData);
}

template<typename BlockType>
void FFTDataGenerator<BlockType>::changeOrder(FFTOrder newOrder)
{
    order = newOrder;
    auto fftSize = getFFTSize();
    
    forwardFFT = std::make_unique<juce::dsp::FFT>(order);
    window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris);
    
    fftData.clear();
    fftData.resize(fftSize * 2, 0);

    fftDataFifo.prepare(fftData.size());
}

template<typename BlockType>
int FFTDataGenerator<BlockType>::getNumAvailableFFTDataBlocks() const { return fftDataFifo.getNumAvailableForReading();}

template<typename BlockType>
int FFTDataGenerator<BlockType>::getFFTSize() const { return 1 << order; }

template<typename BlockType>
bool FFTDataGenerator<BlockType>::getFFTData(BlockType& fftData) { return fftDataFifo.pull(fftData); }
