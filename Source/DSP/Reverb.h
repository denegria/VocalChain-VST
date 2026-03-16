#pragma once
#include <JuceHeader.h>

namespace DSP {

class ReverbProcessor
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setSize(float s) { params.roomSize = juce::jlimit(0.0f, 1.0f, s); reverb.setParameters(params); }
    void setDamping(float d) { params.damping = juce::jlimit(0.0f, 1.0f, d); reverb.setParameters(params); }
    void setMix(float m) { params.wetLevel = m; params.dryLevel = 1.0f - m; reverb.setParameters(params); }
    void setBypass(bool b) { bypassed = b; }

private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
    bool bypassed = false;
};

} // namespace DSP
