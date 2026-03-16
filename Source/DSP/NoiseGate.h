#pragma once
#include <JuceHeader.h>

namespace DSP {

class NoiseGate
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setThreshold(float dB) { threshold = juce::Decibels::decibelsToGain(dB); }
    void setAttack(float ms) { attackTime = ms; }
    void setRelease(float ms) { releaseTime = ms; }
    void setBypass(bool b) { bypassed = b; }

private:
    float threshold = 0.01f;
    float attackTime = 1.0f;
    float releaseTime = 50.0f;
    float envelope = 0.0f;
    double sampleRate = 44100.0;
    bool bypassed = false;
};

} // namespace DSP
