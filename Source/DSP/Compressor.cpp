#include "Compressor.h"

namespace DSP {

void Compressor::prepare(double sr, int /*samplesPerBlock*/)
{
    sampleRate = sr;
    envelope = 0.0f;
}

void Compressor::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed) return;

    const float attackCoeff = std::exp(-1.0f / (float(sampleRate) * attackMs * 0.001f));
    const float releaseCoeff = std::exp(-1.0f / (float(sampleRate) * releaseMs * 0.001f));
    const float makeup = juce::Decibels::decibelsToGain(makeupGain);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // Peak detection across channels
        float peak = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            peak = std::max(peak, std::abs(buffer.getSample(ch, sample)));

        float peakDb = juce::Decibels::gainToDecibels(peak, -100.0f);

        // Envelope with attack/release
        if (peakDb > envelope)
            envelope = attackCoeff * envelope + (1.0f - attackCoeff) * peakDb;
        else
            envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * peakDb;

        // Gain computation
        float gainDb = 0.0f;
        if (envelope > threshold)
            gainDb = (threshold - envelope) * (1.0f - 1.0f / ratio);

        gainReduction = gainDb;
        float gain = juce::Decibels::decibelsToGain(gainDb) * makeup;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.setSample(ch, sample, buffer.getSample(ch, sample) * gain);
    }
}

void Compressor::reset()
{
    envelope = 0.0f;
    gainReduction = 0.0f;
}

} // namespace DSP
