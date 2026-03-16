cmake_minimum_required(VERSION 3.22)
project(VocalChain VERSION 1.0.0)

# Add JUCE
add_subdirectory(JUCE)

# Plugin target
juce_add_plugin(VocalChain
    COMPANY_NAME "VocalChain"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Vchn
    PLUGIN_CODE Vchn
    FORMATS VST3 Standalone
    PRODUCT_NAME "VocalChain"
    DESCRIPTION "Artist-inspired vocal chain presets"
)

# Source files
target_sources(VocalChain PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/DSP/NoiseGate.cpp
    Source/DSP/PitchCorrector.cpp
    Source/DSP/Equalizer.cpp
    Source/DSP/Compressor.cpp
    Source/DSP/DeEsser.cpp
    Source/DSP/Saturation.cpp
    Source/DSP/DelayLine.cpp
    Source/DSP/Reverb.cpp
    Source/Presets/PresetManager.cpp
)

# JUCE modules
target_compile_definitions(VocalChain PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(VocalChain PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Generate JuceHeader.h
juce_generate_juce_header(VocalChain)
