#include "Equalizer.h"

namespace DSP {

void Equalizer::prepare(double sr, int samplesPerBlock)
{
    sampleRate = sr;
    juce::dsp::ProcessSpec spec{ sr, (juce::uint32)samplesPerBlock, 2 };
    lowShelf.prepare(spec);
    midPeak.prepare(spec);
    highShelf.prepare(spec);
    updateFilters();
}

void Equalizer::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed) return;
    updateFilters();

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    lowShelf.process(ctx);
    midPeak.process(ctx);
    highShelf.process(ctx);
}

void Equalizer::updateFilters()
{
    auto lowCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, lowFreq, 0.707f,
        juce::Decibels::decibelsToGain(lowGain));
    auto midCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreq, 1.0f,
        juce::Decibels::decibelsToGain(midGain));
    auto highCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, highFreq, 0.707f,
        juce::Decibels::decibelsToGain(highGain));

    *lowShelf.state = *lowCoeffs;
    *midPeak.state = *midCoeffs;
    *highShelf.state = *highCoeffs;
}

void Equalizer::reset()
{
    lowShelf.reset();
    midPeak.reset();
    highShelf.reset();
}

} // namespace DSP
