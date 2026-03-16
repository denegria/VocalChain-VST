#include "PluginProcessor.h"
#include "PluginEditor.h"

VocalChainProcessor::VocalChainProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

VocalChainProcessor::~VocalChainProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout VocalChainProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // ── Master ──
    params.push_back(std::make_unique<juce::AudioParameterFloat>("dry_wet", "Dry/Wet", 0.0f, 1.0f, 1.0f));

    // ── Voice Tuner Macros ──
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro_brightness", "Brightness", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro_body", "Body", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro_intensity", "Intensity", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro_correction_speed", "Correction Speed", 0.0f, 1.0f, 0.5f));

    // ── Gate ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("gate_bypass", "Gate Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gate_threshold", "Gate Threshold", -80.0f, 0.0f, -40.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gate_attack", "Gate Attack", 0.1f, 20.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gate_release", "Gate Release", 5.0f, 500.0f, 50.0f));

    // ── Pitch ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("pitch_bypass", "Pitch Bypass", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("pitch_speed", "Pitch Speed", 0.0f, 50.0f, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("pitch_mix", "Pitch Mix", 0.0f, 1.0f, 1.0f));

    // ── EQ ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("eq_bypass", "EQ Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_low_gain", "EQ Low Gain", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_low_freq", "EQ Low Freq", 50.0f, 500.0f, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_mid_gain", "EQ Mid Gain", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_mid_freq", "EQ Mid Freq", 500.0f, 8000.0f, 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_high_gain", "EQ High Gain", -12.0f, 12.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("eq_high_freq", "EQ High Freq", 4000.0f, 16000.0f, 8000.0f));

    // ── Compressor ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("comp_bypass", "Comp Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_threshold", "Comp Threshold", -60.0f, 0.0f, -18.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_ratio", "Comp Ratio", 1.0f, 20.0f, 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_attack", "Comp Attack", 0.1f, 100.0f, 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_release", "Comp Release", 10.0f, 500.0f, 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("comp_makeup", "Comp Makeup", 0.0f, 24.0f, 0.0f));

    // ── De-Esser ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("deess_bypass", "De-Ess Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("deess_freq", "De-Ess Freq", 3000.0f, 12000.0f, 7000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("deess_threshold", "De-Ess Threshold", -40.0f, 0.0f, -20.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("deess_reduction", "De-Ess Reduction", -18.0f, 0.0f, -6.0f));

    // ── Saturation ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("sat_bypass", "Sat Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("sat_drive", "Sat Drive", 0.0f, 1.0f, 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("sat_mix", "Sat Mix", 0.0f, 1.0f, 0.5f));

    // ── Delay ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("delay_bypass", "Delay Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delay_time", "Delay Time", 1.0f, 2000.0f, 250.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delay_feedback", "Delay Feedback", 0.0f, 0.9f, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("delay_mix", "Delay Mix", 0.0f, 1.0f, 0.2f));

    // ── Reverb ──
    params.push_back(std::make_unique<juce::AudioParameterBool>("reverb_bypass", "Reverb Bypass", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_size", "Reverb Size", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_damping", "Reverb Damping", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("reverb_mix", "Reverb Mix", 0.0f, 1.0f, 0.2f));

    return { params.begin(), params.end() };
}

void VocalChainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    noiseGate.prepare(sampleRate, samplesPerBlock);
    pitchCorrector.prepare(sampleRate, samplesPerBlock);
    equalizer.prepare(sampleRate, samplesPerBlock);
    compressor.prepare(sampleRate, samplesPerBlock);
    deEsser.prepare(sampleRate, samplesPerBlock);
    saturation.prepare(sampleRate, samplesPerBlock);
    delayLine.prepare(sampleRate, samplesPerBlock);
    reverb.prepare(sampleRate, samplesPerBlock);
}

void VocalChainProcessor::releaseResources()
{
    noiseGate.reset();
    pitchCorrector.reset();
    equalizer.reset();
    compressor.reset();
    deEsser.reset();
    saturation.reset();
    delayLine.reset();
    reverb.reset();
}

void VocalChainProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // Input metering
    float inLevel = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        inLevel = std::max(inLevel, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    inputLevel.store(inLevel);

    // Keep dry copy for dry/wet
    dryWetMix = *apvts.getRawParameterValue("dry_wet");
    juce::AudioBuffer<float> dryBuffer;
    if (dryWetMix < 0.99f)
    {
        dryBuffer.makeCopyOf(buffer);
    }

    // Update DSP from parameters
    noiseGate.setBypass(*apvts.getRawParameterValue("gate_bypass") > 0.5f);
    noiseGate.setThreshold(*apvts.getRawParameterValue("gate_threshold"));
    noiseGate.setAttack(*apvts.getRawParameterValue("gate_attack"));
    noiseGate.setRelease(*apvts.getRawParameterValue("gate_release"));

    pitchCorrector.setBypass(*apvts.getRawParameterValue("pitch_bypass") > 0.5f);
    pitchCorrector.setSpeed(*apvts.getRawParameterValue("pitch_speed"));
    pitchCorrector.setMix(*apvts.getRawParameterValue("pitch_mix"));

    equalizer.setBypass(*apvts.getRawParameterValue("eq_bypass") > 0.5f);
    equalizer.setLowGain(*apvts.getRawParameterValue("eq_low_gain"));
    equalizer.setLowFreq(*apvts.getRawParameterValue("eq_low_freq"));
    equalizer.setMidGain(*apvts.getRawParameterValue("eq_mid_gain"));
    equalizer.setMidFreq(*apvts.getRawParameterValue("eq_mid_freq"));
    equalizer.setHighGain(*apvts.getRawParameterValue("eq_high_gain"));
    equalizer.setHighFreq(*apvts.getRawParameterValue("eq_high_freq"));

    compressor.setBypass(*apvts.getRawParameterValue("comp_bypass") > 0.5f);
    compressor.setThreshold(*apvts.getRawParameterValue("comp_threshold"));
    compressor.setRatio(*apvts.getRawParameterValue("comp_ratio"));
    compressor.setAttack(*apvts.getRawParameterValue("comp_attack"));
    compressor.setRelease(*apvts.getRawParameterValue("comp_release"));
    compressor.setMakeupGain(*apvts.getRawParameterValue("comp_makeup"));

    deEsser.setBypass(*apvts.getRawParameterValue("deess_bypass") > 0.5f);
    deEsser.setFrequency(*apvts.getRawParameterValue("deess_freq"));
    deEsser.setThreshold(*apvts.getRawParameterValue("deess_threshold"));
    deEsser.setReduction(*apvts.getRawParameterValue("deess_reduction"));

    saturation.setBypass(*apvts.getRawParameterValue("sat_bypass") > 0.5f);
    saturation.setDrive(*apvts.getRawParameterValue("sat_drive"));
    saturation.setMix(*apvts.getRawParameterValue("sat_mix"));

    delayLine.setBypass(*apvts.getRawParameterValue("delay_bypass") > 0.5f);
    delayLine.setTime(*apvts.getRawParameterValue("delay_time"));
    delayLine.setFeedback(*apvts.getRawParameterValue("delay_feedback"));
    delayLine.setMix(*apvts.getRawParameterValue("delay_mix"));

    reverb.setBypass(*apvts.getRawParameterValue("reverb_bypass") > 0.5f);
    reverb.setSize(*apvts.getRawParameterValue("reverb_size"));
    reverb.setDamping(*apvts.getRawParameterValue("reverb_damping"));
    reverb.setMix(*apvts.getRawParameterValue("reverb_mix"));

    // ── Process chain ──
    noiseGate.process(buffer);
    pitchCorrector.process(buffer);
    equalizer.process(buffer);
    compressor.process(buffer);
    deEsser.process(buffer);
    saturation.process(buffer);
    delayLine.process(buffer);
    reverb.process(buffer);

    // Dry/wet mixing
    if (dryWetMix < 0.99f)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* wet = buffer.getWritePointer(ch);
            auto* dry = dryBuffer.getReadPointer(ch);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                wet[i] = dry[i] * (1.0f - dryWetMix) + wet[i] * dryWetMix;
        }
    }

    // Output metering
    float outLevel = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        outLevel = std::max(outLevel, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    outputLevel.store(outLevel);
}

juce::AudioProcessorEditor* VocalChainProcessor::createEditor()
{
    return new VocalChainEditor(*this);
}

void VocalChainProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VocalChainProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

// JUCE plugin entry point
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VocalChainProcessor();
}
