#pragma once
#include <JuceHeader.h>
#include "DSP/NoiseGate.h"
#include "DSP/PitchCorrector.h"
#include "DSP/Equalizer.h"
#include "DSP/Compressor.h"
#include "DSP/DeEsser.h"
#include "DSP/Saturation.h"
#include "DSP/DelayLine.h"
#include "DSP/Reverb.h"
#include "Presets/PresetManager.h"

class VocalChainProcessor : public juce::AudioProcessor
{
public:
    VocalChainProcessor();
    ~VocalChainProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "VocalChain"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    PresetManager& getPresetManager() { return presetManager; }

    // DSP module access for UI metering
    float getInputLevel() const { return inputLevel.load(); }
    float getOutputLevel() const { return outputLevel.load(); }

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // DSP Chain (in processing order)
    DSP::NoiseGate noiseGate;
    DSP::PitchCorrector pitchCorrector;
    DSP::Equalizer equalizer;
    DSP::Compressor compressor;
    DSP::DeEsser deEsser;
    DSP::Saturation saturation;
    DSP::DelayLine delayLine;
    DSP::ReverbProcessor reverb;

    PresetManager presetManager;

    // Metering
    std::atomic<float> inputLevel{ 0.0f };
    std::atomic<float> outputLevel{ 0.0f };

    // Dry/wet mix
    float dryWetMix = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalChainProcessor)
};
