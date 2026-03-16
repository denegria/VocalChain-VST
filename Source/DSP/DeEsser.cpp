#include "DeEsser.h"

namespace DSP {

void DeEsser::prepare(double sr, int samplesPerBlock)
{
    sampleRate = sr;
    juce::dsp::ProcessSpec spec{ sr, (juce::uint32)samplesPerBlock, 2 };
    bandFilter.prepare(spec);
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeBandPass(sr, frequency, 2.0f);
    *bandFilter.state = *coeffs;
    envelope = 0.0f;
}

void DeEsser::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed) return;

    // Update filter
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, frequency, 2.0f);
    *bandFilter.state = *coeffs;

    // Detect sibilance in the band
    juce::AudioBuffer<float> sidechain(buffer.getNumChannels(), buffer.getNumSamples());
    sidechain.makeCopyOf(buffer);

    juce::dsp::AudioBlock<float> scBlock(sidechain);
    juce::dsp::ProcessContextReplacing<float> scCtx(scBlock);
    bandFilter.process(scCtx);

    const float releaseCoeff = std::exp(-1.0f / (float(sampleRate) * 0.01f));
    const float threshLinear = juce::Decibels::decibelsToGain(threshold);
    const float reductionLinear = juce::Decibels::decibelsToGain(reduction);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float scPeak = 0.0f;
        for (int ch = 0; ch < sidechain.getNumChannels(); ++ch)
            scPeak = std::max(scPeak, std::abs(sidechain.getSample(ch, sample)));

        envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * scPeak;

        if (envelope > threshLinear)
        {
            float gain = threshLinear / envelope;
            gain = std::max(gain, reductionLinear);
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                buffer.setSample(ch, sample, buffer.getSample(ch, sample) * gain);
        }
    }
}

void DeEsser::reset()
{
    bandFilter.reset();
    envelope = 0.0f;
}

} // namespace DSP
