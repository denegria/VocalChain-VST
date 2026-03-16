#include "Reverb.h"

namespace DSP {

void ReverbProcessor::prepare(double sampleRate, int /*samplesPerBlock*/)
{
    reverb.setSampleRate(sampleRate);
    params.roomSize = 0.5f;
    params.damping = 0.5f;
    params.wetLevel = 0.2f;
    params.dryLevel = 0.8f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;
    reverb.setParameters(params);
}

void ReverbProcessor::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed) return;

    if (buffer.getNumChannels() >= 2)
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1),
                            buffer.getNumSamples());
    else if (buffer.getNumChannels() == 1)
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
}

void ReverbProcessor::reset()
{
    reverb.reset();
}

} // namespace DSP
