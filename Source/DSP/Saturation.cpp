#include "Saturation.h"

namespace DSP {

void Saturation::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed || drive <= 0.0f) return;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float dry = data[i];
            float wet;

            switch (satType)
            {
                case 0:  wet = processTape(dry); break;
                case 1:  wet = processTube(dry); break;
                case 2:  wet = processDigital(dry); break;
                default: wet = processTape(dry); break;
            }

            data[i] = dry * (1.0f - mix) + wet * mix;
        }
    }
}

float Saturation::processTape(float x)
{
    // Soft clipping — tanh-based tape saturation
    float driven = x * (1.0f + drive * 4.0f);
    return std::tanh(driven) * 0.9f;
}

float Saturation::processTube(float x)
{
    // Asymmetric tube-style — even harmonics
    float driven = x * (1.0f + drive * 3.0f);
    if (driven >= 0.0f)
        return 1.0f - std::exp(-driven);
    else
        return -1.0f + std::exp(driven);
}

float Saturation::processDigital(float x)
{
    // Hard clipper with drive
    float driven = x * (1.0f + drive * 6.0f);
    return juce::jlimit(-0.95f, 0.95f, driven);
}

} // namespace DSP
