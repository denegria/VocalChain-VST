#pragma once
#include <JuceHeader.h>

struct ArtistPreset
{
    juce::String name;
    juce::String emoji;

    // Gate
    bool gateBypass; float gateThreshold, gateAttack, gateRelease;
    // Pitch
    bool pitchBypass; float pitchSpeed, pitchMix; int pitchKey, pitchScale;
    // EQ
    bool eqBypass;
    float eqLowGain, eqLowFreq, eqMidGain, eqMidFreq, eqHighGain, eqHighFreq;
    // Compressor
    bool compBypass;
    float compThreshold, compRatio, compAttack, compRelease, compMakeup;
    // De-Esser
    bool deessbypass; float deessFreq, deessThreshold, deessReduction;
    // Saturation
    bool satBypass; float satDrive, satMix; int satType;
    // Delay
    bool delayBypass; float delayTime, delayFeedback, delayMix;
    // Reverb
    bool reverbBypass; float reverbSize, reverbDamping, reverbMix;
};

class PresetManager
{
public:
    PresetManager();

    int getNumPresets() const { return (int)presets.size(); }
    const ArtistPreset& getPreset(int index) const { return presets[index]; }
    const ArtistPreset& getPresetByName(const juce::String& name) const;

    void applyPreset(int index, juce::AudioProcessorValueTreeState& apvts);

private:
    std::vector<ArtistPreset> presets;
    void initPresets();
};
