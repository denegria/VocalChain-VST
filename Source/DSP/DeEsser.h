#pragma once
#include <JuceHeader.h>

namespace DSP {

class DeEsser
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setFrequency(float hz) { frequency = hz; }
    void setThreshold(float dB) { threshold = dB; }
    void setReduction(float dB) { reduction = dB; }
    void setBypass(bool b) { bypassed = b; }

private:
    float frequency = 7000.0f;
    float threshold = -20.0f;
    float reduction = -6.0f;
    double sampleRate = 44100.0;
    bool bypassed = false;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                    juce::dsp::IIR::Coefficients<float>> bandFilter;
    float envelope = 0.0f;
};

} // namespace DSP
