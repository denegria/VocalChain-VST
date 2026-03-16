#pragma once
#include <JuceHeader.h>

namespace DSP {

class Compressor
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setThreshold(float dB) { threshold = dB; }
    void setRatio(float r) { ratio = juce::jlimit(1.0f, 20.0f, r); }
    void setAttack(float ms) { attackMs = ms; }
    void setRelease(float ms) { releaseMs = ms; }
    void setMakeupGain(float dB) { makeupGain = dB; }
    void setBypass(bool b) { bypassed = b; }

    float getGainReduction() const { return gainReduction; }

private:
    float threshold = -18.0f;
    float ratio = 4.0f;
    float attackMs = 10.0f;
    float releaseMs = 100.0f;
    float makeupGain = 0.0f;
    double sampleRate = 44100.0;
    bool bypassed = false;

    float envelope = 0.0f;
    float gainReduction = 0.0f;
};

} // namespace DSP
