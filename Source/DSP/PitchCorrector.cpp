#include "PitchCorrector.h"
#include <cmath>

namespace DSP {

void PitchCorrector::prepare(double sr, int samplesPerBlock)
{
    sampleRate = sr;
    analysisSize = 2048;
    hopSize = 512;
    analysisBuffer.resize(analysisSize, 0.0f);
    writePos = 0;
}

void PitchCorrector::process(juce::AudioBuffer<float>& buffer)
{
    if (bypassed || mix <= 0.0f) return;

    // For MVP: simple pitch detection via autocorrelation
    // Full implementation would use phase vocoder for correction
    // This provides the framework — pitch shifting applied as needed

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Process mono (channel 0 drives pitch detection)
    if (numChannels == 0) return;

    auto* data = buffer.getWritePointer(0);

    // Accumulate samples for analysis
    for (int i = 0; i < numSamples; ++i)
    {
        analysisBuffer[writePos] = data[i];
        writePos = (writePos + 1) % analysisSize;
    }

    // Detect pitch
    float detectedPitch = detectPitch(analysisBuffer.data(), analysisSize);

    if (detectedPitch > 50.0f && detectedPitch < 2000.0f)
    {
        float targetPitch = getNearestScaleFreq(detectedPitch);
        float pitchRatio = targetPitch / detectedPitch;

        // Apply correction with speed (lerp toward target)
        float smoothing = std::exp(-1.0f / (float(sampleRate) * correctionSpeed * 0.001f));
        static float currentRatio = 1.0f;
        currentRatio = smoothing * currentRatio + (1.0f - smoothing) * pitchRatio;

        // Simple pitch shift via sample rate interpolation
        // (production version would use PSOLA or phase vocoder)
        if (std::abs(currentRatio - 1.0f) > 0.001f)
        {
            // Simple interpolation-based pitch shift for MVP
            std::vector<float> tempBuffer(numSamples);
            for (int i = 0; i < numSamples; ++i)
            {
                float readPos = float(i) * currentRatio;
                int idx = int(readPos);
                float frac = readPos - float(idx);
                if (idx + 1 < numSamples)
                    tempBuffer[i] = data[idx] * (1.0f - frac) + data[idx + 1] * frac;
                else
                    tempBuffer[i] = data[idx < numSamples ? idx : numSamples - 1];
            }

            // Apply with dry/wet mix
            for (int i = 0; i < numSamples; ++i)
                data[i] = data[i] * (1.0f - mix) + tempBuffer[i] * mix;

            // Copy to other channels
            for (int ch = 1; ch < numChannels; ++ch)
                buffer.copyFrom(ch, 0, data, numSamples);
        }
    }
}

float PitchCorrector::detectPitch(const float* data, int numSamples)
{
    // Autocorrelation-based pitch detection (YIN-simplified)
    const int minPeriod = int(sampleRate / 1000.0); // 1000 Hz max
    const int maxPeriod = int(sampleRate / 60.0);   // 60 Hz min

    if (maxPeriod >= numSamples) return 0.0f;

    float bestCorrelation = 0.0f;
    int bestPeriod = 0;

    for (int period = minPeriod; period < maxPeriod; ++period)
    {
        float correlation = 0.0f;
        float energy1 = 0.0f;
        float energy2 = 0.0f;

        for (int i = 0; i < numSamples - maxPeriod; ++i)
        {
            correlation += data[i] * data[i + period];
            energy1 += data[i] * data[i];
            energy2 += data[i + period] * data[i + period];
        }

        float denom = std::sqrt(energy1 * energy2);
        if (denom > 0.0001f)
        {
            correlation /= denom;
            if (correlation > bestCorrelation)
            {
                bestCorrelation = correlation;
                bestPeriod = period;
            }
        }
    }

    if (bestCorrelation > 0.5f && bestPeriod > 0)
        return float(sampleRate) / float(bestPeriod);

    return 0.0f;
}

float PitchCorrector::getNearestScaleFreq(float freq)
{
    // Convert frequency to MIDI note number
    float midiNote = 69.0f + 12.0f * std::log2(freq / 440.0f);
    int noteInOctave = int(std::round(midiNote)) % 12;
    if (noteInOctave < 0) noteInOctave += 12;

    // Get scale degrees relative to root key
    const auto& scale = (scaleType == 1) ? majorScale :
                        (scaleType == 2) ? minorScale :
                        std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    int relativeNote = (noteInOctave - rootKey + 12) % 12;

    // Find nearest scale degree
    int bestDegree = relativeNote;
    int bestDist = 12;
    for (int degree : scale)
    {
        int dist = std::min(std::abs(relativeNote - degree),
                           12 - std::abs(relativeNote - degree));
        if (dist < bestDist)
        {
            bestDist = dist;
            bestDegree = degree;
        }
    }

    // Convert back to frequency
    int targetNote = (bestDegree + rootKey) % 12;
    int originalNote = int(std::round(midiNote));
    int shift = targetNote - noteInOctave;
    if (shift > 6) shift -= 12;
    if (shift < -6) shift += 12;

    float targetMidi = float(originalNote + shift);
    return 440.0f * std::pow(2.0f, (targetMidi - 69.0f) / 12.0f);
}

void PitchCorrector::reset()
{
    std::fill(analysisBuffer.begin(), analysisBuffer.end(), 0.0f);
    writePos = 0;
}

} // namespace DSP
