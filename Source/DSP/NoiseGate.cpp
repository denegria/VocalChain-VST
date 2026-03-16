#include "NoiseGate.h"

namespace DSP {

void NoiseGate::prepare(double sr, int /*samplesPerBlock*/)
{
    sampleRate = sr;
    envelope = 0.0f;
}

void NoiseGate::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed) return;

    const float attackCoeff = std::exp(-1.0f / (float(sampleRate) * attackTime * 0.001f));
    const float releaseCoeff = std::exp(-1.0f / (float(sampleRate) * releaseTime * 0.001f));

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // Get max absolute value across channels
        float maxVal = 0.0f;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            maxVal = std::max(maxVal, std::abs(buffer.getSample(ch, sample)));

        // Envelope follower
        if (maxVal > envelope)
            envelope = attackCoeff * envelope + (1.0f - attackCoeff) * maxVal;
        else
            envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * maxVal;

        // Gate: smooth gain reduction
        float gain = (envelope > threshold) ? 1.0f : (envelope / threshold);
        gain = std::max(gain, 0.0f);

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.setSample(ch, sample, buffer.getSample(ch, sample) * gain);
    }
}

void NoiseGate::reset()
{
    envelope = 0.0f;
}

} // namespace DSP
