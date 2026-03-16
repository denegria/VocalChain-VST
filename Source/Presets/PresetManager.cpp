#include "PresetManager.h"

PresetManager::PresetManager()
{
    initPresets();
}

const ArtistPreset& PresetManager::getPresetByName(const juce::String& name) const
{
    for (auto& p : presets)
        if (p.name == name) return p;
    return presets[0];
}

void PresetManager::applyPreset(int index, juce::AudioProcessorValueTreeState& apvts)
{
    if (index < 0 || index >= (int)presets.size()) return;
    const auto& p = presets[index];

    // Gate
    if (auto* param = apvts.getParameter("gate_bypass"))   param->setValueNotifyingHost(p.gateBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("gate_threshold")) param->setValueNotifyingHost(apvts.getParameterRange("gate_threshold").convertTo0to1(p.gateThreshold));
    if (auto* param = apvts.getParameter("gate_attack"))    param->setValueNotifyingHost(apvts.getParameterRange("gate_attack").convertTo0to1(p.gateAttack));
    if (auto* param = apvts.getParameter("gate_release"))   param->setValueNotifyingHost(apvts.getParameterRange("gate_release").convertTo0to1(p.gateRelease));

    // Pitch
    if (auto* param = apvts.getParameter("pitch_bypass"))  param->setValueNotifyingHost(p.pitchBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("pitch_speed"))   param->setValueNotifyingHost(apvts.getParameterRange("pitch_speed").convertTo0to1(p.pitchSpeed));
    if (auto* param = apvts.getParameter("pitch_mix"))     param->setValueNotifyingHost(apvts.getParameterRange("pitch_mix").convertTo0to1(p.pitchMix));

    // EQ
    if (auto* param = apvts.getParameter("eq_bypass"))     param->setValueNotifyingHost(p.eqBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("eq_low_gain"))   param->setValueNotifyingHost(apvts.getParameterRange("eq_low_gain").convertTo0to1(p.eqLowGain));
    if (auto* param = apvts.getParameter("eq_low_freq"))   param->setValueNotifyingHost(apvts.getParameterRange("eq_low_freq").convertTo0to1(p.eqLowFreq));
    if (auto* param = apvts.getParameter("eq_mid_gain"))   param->setValueNotifyingHost(apvts.getParameterRange("eq_mid_gain").convertTo0to1(p.eqMidGain));
    if (auto* param = apvts.getParameter("eq_mid_freq"))   param->setValueNotifyingHost(apvts.getParameterRange("eq_mid_freq").convertTo0to1(p.eqMidFreq));
    if (auto* param = apvts.getParameter("eq_high_gain"))  param->setValueNotifyingHost(apvts.getParameterRange("eq_high_gain").convertTo0to1(p.eqHighGain));
    if (auto* param = apvts.getParameter("eq_high_freq"))  param->setValueNotifyingHost(apvts.getParameterRange("eq_high_freq").convertTo0to1(p.eqHighFreq));

    // Compressor
    if (auto* param = apvts.getParameter("comp_bypass"))    param->setValueNotifyingHost(p.compBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("comp_threshold")) param->setValueNotifyingHost(apvts.getParameterRange("comp_threshold").convertTo0to1(p.compThreshold));
    if (auto* param = apvts.getParameter("comp_ratio"))     param->setValueNotifyingHost(apvts.getParameterRange("comp_ratio").convertTo0to1(p.compRatio));
    if (auto* param = apvts.getParameter("comp_attack"))    param->setValueNotifyingHost(apvts.getParameterRange("comp_attack").convertTo0to1(p.compAttack));
    if (auto* param = apvts.getParameter("comp_release"))   param->setValueNotifyingHost(apvts.getParameterRange("comp_release").convertTo0to1(p.compRelease));
    if (auto* param = apvts.getParameter("comp_makeup"))    param->setValueNotifyingHost(apvts.getParameterRange("comp_makeup").convertTo0to1(p.compMakeup));

    // De-Esser
    if (auto* param = apvts.getParameter("deess_bypass"))    param->setValueNotifyingHost(p.deessbypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("deess_freq"))      param->setValueNotifyingHost(apvts.getParameterRange("deess_freq").convertTo0to1(p.deessFreq));
    if (auto* param = apvts.getParameter("deess_threshold")) param->setValueNotifyingHost(apvts.getParameterRange("deess_threshold").convertTo0to1(p.deessThreshold));
    if (auto* param = apvts.getParameter("deess_reduction")) param->setValueNotifyingHost(apvts.getParameterRange("deess_reduction").convertTo0to1(p.deessReduction));

    // Saturation
    if (auto* param = apvts.getParameter("sat_bypass")) param->setValueNotifyingHost(p.satBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("sat_drive"))  param->setValueNotifyingHost(apvts.getParameterRange("sat_drive").convertTo0to1(p.satDrive));
    if (auto* param = apvts.getParameter("sat_mix"))    param->setValueNotifyingHost(apvts.getParameterRange("sat_mix").convertTo0to1(p.satMix));

    // Delay
    if (auto* param = apvts.getParameter("delay_bypass"))   param->setValueNotifyingHost(p.delayBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("delay_time"))     param->setValueNotifyingHost(apvts.getParameterRange("delay_time").convertTo0to1(p.delayTime));
    if (auto* param = apvts.getParameter("delay_feedback")) param->setValueNotifyingHost(apvts.getParameterRange("delay_feedback").convertTo0to1(p.delayFeedback));
    if (auto* param = apvts.getParameter("delay_mix"))      param->setValueNotifyingHost(apvts.getParameterRange("delay_mix").convertTo0to1(p.delayMix));

    // Reverb
    if (auto* param = apvts.getParameter("reverb_bypass"))  param->setValueNotifyingHost(p.reverbBypass ? 1.0f : 0.0f);
    if (auto* param = apvts.getParameter("reverb_size"))    param->setValueNotifyingHost(apvts.getParameterRange("reverb_size").convertTo0to1(p.reverbSize));
    if (auto* param = apvts.getParameter("reverb_damping")) param->setValueNotifyingHost(apvts.getParameterRange("reverb_damping").convertTo0to1(p.reverbDamping));
    if (auto* param = apvts.getParameter("reverb_mix"))     param->setValueNotifyingHost(apvts.getParameterRange("reverb_mix").convertTo0to1(p.reverbMix));
}

void PresetManager::initPresets()
{
    // ═══════════════════════════════════════════
    //  YEAT — Hard auto-tune, gritty, aggressive
    // ═══════════════════════════════════════════
    presets.push_back({
        "Yeat", "🔴",
        // Gate
        false, -40.0f, 1.0f, 50.0f,
        // Pitch: hard tune, instant snap
        false, 2.0f, 1.0f, 0, 0,
        // EQ: presence boost, cut lows, air
        false, -3.0f, 100.0f, 4.0f, 3000.0f, 3.0f, 8000.0f,
        // Comp: aggressive, fast attack
        false, -20.0f, 4.0f, 5.0f, 80.0f, 4.0f,
        // De-Esser
        false, 7000.0f, -18.0f, -6.0f,
        // Saturation: tube, medium drive
        false, 0.4f, 0.5f, 1,
        // Delay: 1/8 note ~188ms, low feedback
        false, 188.0f, 0.12f, 0.15f,
        // Reverb: small room, short
        false, 0.3f, 0.5f, 0.2f,
    });

    // ═══════════════════════════════════════════
    //  DRAKE — Warm, intimate, smooth
    // ═══════════════════════════════════════════
    presets.push_back({
        "Drake", "🟣",
        // Gate
        false, -42.0f, 2.0f, 80.0f,
        // Pitch: natural correction
        false, 25.0f, 0.8f, 0, 0,
        // EQ: warm body, slight cut 3k, air
        false, 3.0f, 200.0f, -2.0f, 3000.0f, 2.0f, 10000.0f,
        // Comp: moderate leveling
        false, -18.0f, 3.0f, 10.0f, 100.0f, 3.0f,
        // De-Esser: active
        false, 6500.0f, -20.0f, -5.0f,
        // Saturation: light tape warmth
        false, 0.2f, 0.3f, 0,
        // Delay: 1/4 note ~375ms, moderate
        false, 375.0f, 0.2f, 0.2f,
        // Reverb: medium hall
        false, 0.55f, 0.4f, 0.25f,
    });

    // ═══════════════════════════════════════════
    //  JUICE WRLD — Melodic, spacious, bright
    // ═══════════════════════════════════════════
    presets.push_back({
        "Juice WRLD", "🟡",
        // Gate
        false, -42.0f, 1.5f, 60.0f,
        // Pitch: medium-fast, melodic
        false, 12.0f, 0.9f, 0, 0,
        // EQ: bright, cut muddiness
        false, -2.0f, 300.0f, 2.0f, 5000.0f, 3.0f, 8000.0f,
        // Comp: medium
        false, -16.0f, 3.5f, 8.0f, 120.0f, 3.0f,
        // De-Esser
        false, 7500.0f, -22.0f, -4.0f,
        // Saturation: light tube
        false, 0.2f, 0.3f, 1,
        // Delay: dotted 1/4 ~562ms, spacious
        false, 562.0f, 0.25f, 0.2f,
        // Reverb: large hall, spacious
        false, 0.75f, 0.3f, 0.3f,
    });

    // ═══════════════════════════════════════════
    //  KANYE — Punchy, mid-forward, aggressive
    // ═══════════════════════════════════════════
    presets.push_back({
        "Kanye", "⚫",
        // Gate
        false, -38.0f, 1.0f, 40.0f,
        // Pitch: 808s heavy tune
        false, 5.0f, 1.0f, 0, 0,
        // EQ: aggressive mid-forward
        false, 0.0f, 150.0f, 5.0f, 2000.0f, -2.0f, 5000.0f,
        // Comp: heavy, punchy
        false, -22.0f, 6.0f, 3.0f, 60.0f, 6.0f,
        // De-Esser
        false, 6000.0f, -16.0f, -8.0f,
        // Saturation: high drive, digital
        false, 0.6f, 0.5f, 2,
        // Delay: 1/8 triplet ~125ms, low mix
        false, 125.0f, 0.1f, 0.1f,
        // Reverb: tight room / plate
        false, 0.25f, 0.6f, 0.15f,
    });

    // ═══════════════════════════════════════════
    //  LIL UZI VERT — Melodic, bright, energetic
    // ═══════════════════════════════════════════
    presets.push_back({
        "Lil Uzi Vert", "🔵",
        // Gate
        false, -40.0f, 1.0f, 50.0f,
        // Pitch: fast correction, melodic
        false, 8.0f, 0.95f, 0, 0,
        // EQ: bright + airy, slight warmth
        false, 1.5f, 200.0f, 1.0f, 4000.0f, 4.0f, 7000.0f,
        // Comp: moderate-fast
        false, -18.0f, 4.0f, 5.0f, 90.0f, 4.0f,
        // De-Esser
        false, 7000.0f, -20.0f, -5.0f,
        // Saturation: medium tube
        false, 0.35f, 0.4f, 1,
        // Delay: 1/8 note ~188ms, stereo spread
        false, 188.0f, 0.15f, 0.18f,
        // Reverb: medium plate
        false, 0.45f, 0.4f, 0.25f,
    });
}
