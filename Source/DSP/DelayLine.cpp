#include "DelayLine.h"

namespace DSP {

void DelayLine::prepare(double sr, int /*samplesPerBlock*/)
{
    sampleRate = sr;
    maxDelaySamples = int(sr * 2.0); // 2 seconds max
    delayBuffer.resize(2);
    for (auto& ch : delayBuffer)
        ch.resize(maxDelaySamples, 0.0f);
    writePosition = 0;
}

void DelayLine::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed || mix <= 0.0f) return;

    int delaySamples = juce::jlimit(1, maxDelaySamples - 1,
                                     int(delayTimeMs * 0.001f * sampleRate));

    for (int ch = 0; ch < std::min(buffer.getNumChannels(), 2); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            int readPos = (writePosition - delaySamples + maxDelaySamples) % maxDelaySamples;
            float delayedSample = delayBuffer[ch][readPos];

            // Write input + feedback
            delayBuffer[ch][writePosition] = data[i] + delayedSample * feedback;

            // Mix
            data[i] = data[i] * (1.0f - mix) + delayedSample * mix;

            if (ch == buffer.getNumChannels() - 1)
                writePosition = (writePosition + 1) % maxDelaySamples;
        }
    }
}

void DelayLine::reset()
{
    for (auto& ch : delayBuffer)
        std::fill(ch.begin(), ch.end(), 0.0f);
    writePosition = 0;
}

} // namespace DSP
