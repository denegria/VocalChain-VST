#pragma once
#include <JuceHeader.h>

namespace DSP {

class Saturation
{
public:
    void prepare(double sampleRate, int samplesPerBlock) { (void)sampleRate; (void)samplesPerBlock; }
    void process(juce::AudioBuffer<float>& buffer);
    void reset() {}

    void setDrive(float d) { drive = juce::jlimit(0.0f, 1.0f, d); }
    void setMix(float m) { mix = juce::jlimit(0.0f, 1.0f, m); }
    // 0=tape, 1=tube, 2=digital
    void setType(int t) { satType = juce::jlimit(0, 2, t); }
    void setBypass(bool b) { bypassed = b; }

private:
    float drive = 0.3f;
    float mix = 0.5f;
    int satType = 0; // tape
    bool bypassed = false;

    float processTape(float x);
    float processTube(float x);
    float processDigital(float x);
};

} // namespace DSP
