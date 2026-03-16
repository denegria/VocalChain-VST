#pragma once
#include <JuceHeader.h>

namespace DSP {

class DelayLine
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setTime(float ms) { delayTimeMs = ms; }
    void setFeedback(float f) { feedback = juce::jlimit(0.0f, 0.9f, f); }
    void setMix(float m) { mix = juce::jlimit(0.0f, 1.0f, m); }
    void setBypass(bool b) { bypassed = b; }

private:
    float delayTimeMs = 250.0f;
    float feedback = 0.2f;
    float mix = 0.2f;
    double sampleRate = 44100.0;
    bool bypassed = false;

    std::vector<std::vector<float>> delayBuffer;
    int writePosition = 0;
    int maxDelaySamples = 0;
};

} // namespace DSP
