#pragma once
#include <JuceHeader.h>

namespace DSP {

class Equalizer
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setLowGain(float dB) { lowGain = dB; }
    void setLowFreq(float hz) { lowFreq = hz; }
    void setMidGain(float dB) { midGain = dB; }
    void setMidFreq(float hz) { midFreq = hz; }
    void setHighGain(float dB) { highGain = dB; }
    void setHighFreq(float hz) { highFreq = hz; }
    void setBypass(bool b) { bypassed = b; }

private:
    void updateFilters();

    float lowGain = 0.0f, lowFreq = 200.0f;
    float midGain = 0.0f, midFreq = 2000.0f;
    float highGain = 0.0f, highFreq = 8000.0f;
    double sampleRate = 44100.0;
    bool bypassed = false;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                    juce::dsp::IIR::Coefficients<float>> lowShelf, midPeak, highShelf;
};

} // namespace DSP
