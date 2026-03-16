#pragma once
#include <JuceHeader.h>

namespace DSP {

// Simple pitch correction using JUCE's built-in pitch detection
// and phase vocoder for shifting to nearest scale degree
class PitchCorrector
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setSpeed(float ms) { correctionSpeed = ms; } // 0=instant, 50=natural
    void setMix(float m) { mix = juce::jlimit(0.0f, 1.0f, m); }
    void setBypass(bool b) { bypassed = b; }

    // Musical key (0=C, 1=C#, ... 11=B)
    void setKey(int k) { rootKey = k % 12; }
    // Scale: 0=chromatic, 1=major, 2=minor
    void setScale(int s) { scaleType = s; }

private:
    float correctionSpeed = 10.0f;
    float mix = 1.0f;
    int rootKey = 0;
    int scaleType = 0; // 0=chromatic, 1=major, 2=minor
    double sampleRate = 44100.0;
    bool bypassed = true; // Off by default — not all presets use auto-tune

    // Internal pitch detection state
    std::vector<float> analysisBuffer;
    int analysisSize = 2048;
    int hopSize = 512;
    int writePos = 0;

    float detectPitch(const float* data, int numSamples);
    float getNearestScaleFreq(float freq);

    // Scale definitions (semitone offsets from root)
    const std::vector<int> majorScale = { 0, 2, 4, 5, 7, 9, 11 };
    const std::vector<int> minorScale = { 0, 2, 3, 5, 7, 8, 10 };
};

} // namespace DSP
