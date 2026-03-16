#pragma once
#include "PluginProcessor.h"

class VocalChainEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    explicit VocalChainEditor(VocalChainProcessor&);
    ~VocalChainEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void buildUI();
    void onPresetSelected(int index);
    void toggleAB();
    void saveUserPreset();
    void loadUserPreset();

    VocalChainProcessor& processor;

    // ── Preset Buttons ──
    juce::OwnedArray<juce::TextButton> presetButtons;
    int activePreset = -1;

    // ── Voice Tuner Macros ──
    juce::Slider brightnessSlider, bodySlider, intensitySlider, correctionSpeedSlider;
    juce::Label brightnessLabel, bodyLabel, intensityLabel, correctionLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        brightnessAtt, bodyAtt, intensityAtt, correctionAtt;

    // ── Module bypass ──
    juce::OwnedArray<juce::ToggleButton> bypassButtons;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> bypassAttachments;

    // ── Dry/Wet ──
    juce::Slider dryWetSlider;
    juce::Label dryWetLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAtt;

    // ── A/B Compare ──
    juce::TextButton abButton;
    bool abStateIsA = true;
    juce::ValueTree abSnapshotA, abSnapshotB;

    // ── Save/Load ──
    juce::TextButton saveButton, loadButton;

    // ── Metering ──
    float inputLevel = 0.0f, outputLevel = 0.0f;

    // ══ MODERN COLOR SYSTEM ══
    const juce::Colour bgPrimary   { 0xff0a0a12 };  // Deep black-blue
    const juce::Colour bgSecondary { 0xff111120 };  // Card background
    const juce::Colour bgTertiary  { 0xff1a1a30 };  // Elevated card
    const juce::Colour bgHover     { 0xff222240 };  // Hover state
    
    const juce::Colour accentCyan  { 0xff00d4ff };  // Primary accent
    const juce::Colour accentPurp  { 0xff8b5cf6 };  // Secondary accent
    const juce::Colour accentGrad1 { 0xff0099cc };  // Gradient start
    const juce::Colour accentGrad2 { 0xff00d4ff };  // Gradient end
    
    const juce::Colour textWhite   { 0xfff0f4f8 };
    const juce::Colour textMuted   { 0xff7a839a };
    const juce::Colour textDim     { 0xff4a5070 };
    
    const juce::Colour green       { 0xff22c55e };
    const juce::Colour red         { 0xffef4444 };
    const juce::Colour amber       { 0xfffbbf24 };
    
    const juce::Colour borderSub   { 0xff1e1e38 };

    // Preset gradient colors
    const juce::Colour presetColors[5] = {
        juce::Colour(0xffef4444), // Yeat — red
        juce::Colour(0xffa855f7), // Drake — purple
        juce::Colour(0xfffbbf24), // Juice WRLD — gold
        juce::Colour(0xff94a3b8), // Kanye — slate
        juce::Colour(0xff3b82f6), // Uzi — blue
    };

    // Helper: draw a modern rounded card
    void drawCard(juce::Graphics& g, juce::Rectangle<float> bounds, float cornerRadius = 10.0f);
    void drawGlowCard(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour glowColor);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VocalChainEditor)
};
