#include "PluginEditor.h"

VocalChainEditor::VocalChainEditor(VocalChainProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(800, 560);
    setResizable(false, false);
    buildUI();
    startTimerHz(30);
}

VocalChainEditor::~VocalChainEditor()
{
    stopTimer();
}

// ══════════════════════════════════════════════
//  HELPERS
// ══════════════════════════════════════════════

void VocalChainEditor::drawCard(juce::Graphics& g, juce::Rectangle<float> bounds, float cr)
{
    // Shadow
    g.setColour(juce::Colour(0x18000000));
    g.fillRoundedRectangle(bounds.translated(0, 2).expanded(1), cr);
    // Card fill
    g.setColour(bgSecondary);
    g.fillRoundedRectangle(bounds, cr);
    // Border
    g.setColour(borderSub);
    g.drawRoundedRectangle(bounds, cr, 0.5f);
}

void VocalChainEditor::drawGlowCard(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour glowColor)
{
    // Outer glow
    g.setColour(glowColor.withAlpha(0.06f));
    g.fillRoundedRectangle(bounds.expanded(3), 12.0f);
    // Card
    g.setColour(bgSecondary);
    g.fillRoundedRectangle(bounds, 10.0f);
    // Glow border
    g.setColour(glowColor.withAlpha(0.25f));
    g.drawRoundedRectangle(bounds, 10.0f, 1.0f);
}

// ══════════════════════════════════════════════
//  BUILD UI
// ══════════════════════════════════════════════

void VocalChainEditor::buildUI()
{
    auto& pm = processor.getPresetManager();
    auto& apvts = processor.getAPVTS();

    // ── Preset Buttons ──
    for (int i = 0; i < pm.getNumPresets(); ++i)
    {
        auto* btn = new juce::TextButton(pm.getPreset(i).emoji + " " + pm.getPreset(i).name);
        btn->setColour(juce::TextButton::buttonColourId, bgTertiary);
        btn->setColour(juce::TextButton::textColourOffId, textMuted);
        btn->onClick = [this, i]() { onPresetSelected(i); };
        addAndMakeVisible(btn);
        presetButtons.add(btn);
    }

    // ── Voice Tuner Knobs ──
    auto setupKnob = [this](juce::Slider& slider, juce::Label& label, const juce::String& text)
    {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setColour(juce::Slider::rotarySliderFillColourId, accentCyan);
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, bgTertiary);
        slider.setColour(juce::Slider::thumbColourId, accentCyan);
        addAndMakeVisible(slider);

        label.setText(text, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setFont(juce::FontOptions(9.0f));
        label.setColour(juce::Label::textColourId, textMuted);
        addAndMakeVisible(label);
    };

    setupKnob(brightnessSlider, brightnessLabel, "BRIGHTNESS");
    setupKnob(bodySlider, bodyLabel, "BODY");
    setupKnob(intensitySlider, intensityLabel, "INTENSITY");
    setupKnob(correctionSpeedSlider, correctionLabel, "CORRECTION");

    brightnessAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "macro_brightness", brightnessSlider);
    bodyAtt         = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "macro_body", bodySlider);
    intensityAtt    = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "macro_intensity", intensitySlider);
    correctionAtt   = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "macro_correction_speed", correctionSpeedSlider);

    // ── Module Bypass Toggles ──
    const juce::StringArray moduleNames = { "GATE", "PITCH", "EQ", "COMP", "DE-ESS", "SAT", "DELAY", "REVERB" };
    const juce::StringArray bypassIds   = { "gate_bypass", "pitch_bypass", "eq_bypass", "comp_bypass",
                                             "deess_bypass", "sat_bypass", "delay_bypass", "reverb_bypass" };

    for (int i = 0; i < moduleNames.size(); ++i)
    {
        auto* toggle = new juce::ToggleButton(moduleNames[i]);
        toggle->setColour(juce::ToggleButton::tickColourId, accentCyan);
        toggle->setColour(juce::ToggleButton::textColourId, textWhite);
        addAndMakeVisible(toggle);
        bypassButtons.add(toggle);

        auto att = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, bypassIds[i], *toggle);
        bypassAttachments.push_back(std::move(att));
    }

    // ── Dry/Wet ──
    dryWetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 42, 18);
    dryWetSlider.setColour(juce::Slider::trackColourId, accentCyan);
    dryWetSlider.setColour(juce::Slider::backgroundColourId, bgTertiary);
    dryWetSlider.setColour(juce::Slider::thumbColourId, accentCyan);
    dryWetSlider.setColour(juce::Slider::textBoxTextColourId, textWhite);
    dryWetSlider.setColour(juce::Slider::textBoxBackgroundColourId, bgTertiary);
    dryWetSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(dryWetSlider);

    dryWetLabel.setText("DRY / WET", juce::dontSendNotification);
    dryWetLabel.setFont(juce::FontOptions(9.0f));
    dryWetLabel.setColour(juce::Label::textColourId, textMuted);
    addAndMakeVisible(dryWetLabel);

    dryWetAtt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "dry_wet", dryWetSlider);

    // ── A/B Compare ──
    abButton.setButtonText("A / B");
    abButton.setColour(juce::TextButton::buttonColourId, bgTertiary);
    abButton.setColour(juce::TextButton::textColourOffId, textWhite);
    abButton.onClick = [this]() { toggleAB(); };
    addAndMakeVisible(abButton);

    // ── Save / Load ──
    saveButton.setButtonText("SAVE");
    saveButton.setColour(juce::TextButton::buttonColourId, bgTertiary);
    saveButton.setColour(juce::TextButton::textColourOffId, accentCyan);
    saveButton.onClick = [this]() { saveUserPreset(); };
    addAndMakeVisible(saveButton);

    loadButton.setButtonText("LOAD");
    loadButton.setColour(juce::TextButton::buttonColourId, bgTertiary);
    loadButton.setColour(juce::TextButton::textColourOffId, textMuted);
    loadButton.onClick = [this]() { loadUserPreset(); };
    addAndMakeVisible(loadButton);
}

// ══════════════════════════════════════════════
//  CALLBACKS
// ══════════════════════════════════════════════

void VocalChainEditor::onPresetSelected(int index)
{
    processor.getPresetManager().applyPreset(index, processor.getAPVTS());
    activePreset = index;

    abSnapshotA = processor.getAPVTS().copyState();
    abStateIsA = true;

    for (int i = 0; i < presetButtons.size(); ++i)
    {
        bool active = (i == index);
        presetButtons[i]->setColour(juce::TextButton::buttonColourId,
            active ? presetColors[i].withAlpha(0.18f) : bgTertiary);
        presetButtons[i]->setColour(juce::TextButton::textColourOffId,
            active ? presetColors[i].brighter(0.4f) : textMuted);
    }
    repaint();
}

void VocalChainEditor::toggleAB()
{
    auto& apvts = processor.getAPVTS();
    if (abStateIsA)
    {
        abSnapshotA = apvts.copyState();
        if (abSnapshotB.isValid()) apvts.replaceState(abSnapshotB);
        abStateIsA = false;
        abButton.setColour(juce::TextButton::buttonColourId, accentPurp.withAlpha(0.2f));
        abButton.setButtonText("B / A");
    }
    else
    {
        abSnapshotB = apvts.copyState();
        if (abSnapshotA.isValid()) apvts.replaceState(abSnapshotA);
        abStateIsA = true;
        abButton.setColour(juce::TextButton::buttonColourId, bgTertiary);
        abButton.setButtonText("A / B");
    }
    repaint();
}

void VocalChainEditor::saveUserPreset()
{
    auto chooser = std::make_shared<juce::FileChooser>("Save Preset",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.vcpreset");
    chooser->launchAsync(juce::FileBrowserComponent::saveMode, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file != juce::File{})
        {
            auto state = processor.getAPVTS().copyState();
            std::unique_ptr<juce::XmlElement> xml(state.createXml());
            if (xml) xml->writeTo(file.withFileExtension("vcpreset"));
        }
    });
}

void VocalChainEditor::loadUserPreset()
{
    auto chooser = std::make_shared<juce::FileChooser>("Load Preset",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), "*.vcpreset");
    chooser->launchAsync(juce::FileBrowserComponent::openMode, [this, chooser](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.existsAsFile())
        {
            auto xml = juce::XmlDocument::parse(file);
            if (xml && xml->hasTagName(processor.getAPVTS().state.getType()))
            {
                processor.getAPVTS().replaceState(juce::ValueTree::fromXml(*xml));
                activePreset = -1;
                for (auto* btn : presetButtons)
                {
                    btn->setColour(juce::TextButton::buttonColourId, bgTertiary);
                    btn->setColour(juce::TextButton::textColourOffId, textMuted);
                }
                repaint();
            }
        }
    });
}

// ══════════════════════════════════════════════
//  PAINT — MODERN PREMIUM UI
// ══════════════════════════════════════════════

void VocalChainEditor::paint(juce::Graphics& g)
{
    int w = getWidth();
    int h = getHeight();

    // ── Background: subtle gradient ──
    g.setGradientFill(juce::ColourGradient(
        bgPrimary, 0.0f, 0.0f,
        bgPrimary.brighter(0.03f), (float)w, (float)h, false));
    g.fillAll();

    // ── Top Bar ──
    {
        auto barRect = juce::Rectangle<float>(0, 0, (float)w, 48.0f);
        g.setColour(bgSecondary);
        g.fillRect(barRect);

        // Accent gradient line under bar
        g.setGradientFill(juce::ColourGradient(
            accentGrad1, 0, 48.0f, accentGrad2, (float)w, 48.0f, false));
        g.fillRect(0.0f, 47.0f, (float)w, 1.5f);

        // Logo
        g.setFont(juce::FontOptions(17.0f).withStyle("Bold"));
        g.setColour(accentCyan);
        g.drawText("VOCALCHAIN", 18, 0, 160, 48, juce::Justification::centredLeft);

        // Version pill
        auto pillRect = juce::Rectangle<float>(148.0f, 16.0f, 32.0f, 16.0f);
        g.setColour(bgTertiary);
        g.fillRoundedRectangle(pillRect, 8.0f);
        g.setFont(juce::FontOptions(8.0f));
        g.setColour(textDim);
        g.drawText("v1.0", pillRect, juce::Justification::centred);

        // Right side info
        g.setFont(juce::FontOptions(10.0f));
        g.setColour(textMuted);
        g.drawText("ARTIST VOCAL CHAIN", w - 200, 0, 182, 48, juce::Justification::centredRight);
    }

    // ── Section: PRESETS card ──
    {
        auto cardRect = juce::Rectangle<float>(14.0f, 56.0f, w - 28.0f, 66.0f);
        drawCard(g, cardRect, 10.0f);

        g.setFont(juce::FontOptions(8.5f).withStyle("Bold"));
        g.setColour(textDim);
        g.drawText("SELECT PRESET", 26, 58, 100, 16, juce::Justification::centredLeft);

        // Active preset glow behind button
        if (activePreset >= 0 && activePreset < 5)
        {
            auto* btn = presetButtons[activePreset];
            auto glow = btn->getBounds().toFloat().expanded(3);
            g.setColour(presetColors[activePreset].withAlpha(0.08f));
            g.fillRoundedRectangle(glow, 6.0f);
            g.setColour(presetColors[activePreset].withAlpha(0.2f));
            g.drawRoundedRectangle(glow, 6.0f, 1.0f);
        }
    }

    // ── Section: VOICE TUNER card ──
    {
        auto cardRect = juce::Rectangle<float>(14.0f, 130.0f, w - 28.0f, 126.0f);
        drawCard(g, cardRect, 10.0f);

        g.setFont(juce::FontOptions(8.5f).withStyle("Bold"));
        g.setColour(textDim);
        g.drawText("VOICE TUNER", 26, 132, 100, 16, juce::Justification::centredLeft);

        // Decorative accent dots next to label
        g.setColour(accentCyan.withAlpha(0.4f));
        g.fillEllipse(113.0f, 138.0f, 4.0f, 4.0f);
        g.setColour(accentPurp.withAlpha(0.4f));
        g.fillEllipse(120.0f, 138.0f, 4.0f, 4.0f);
    }

    // ── Section: MODULES card ──
    {
        auto cardRect = juce::Rectangle<float>(14.0f, 264.0f, w - 28.0f, 100.0f);
        drawCard(g, cardRect, 10.0f);

        g.setFont(juce::FontOptions(8.5f).withStyle("Bold"));
        g.setColour(textDim);
        g.drawText("SIGNAL CHAIN MODULES", 26, 266, 160, 16, juce::Justification::centredLeft);
    }

    // ── Section: OUTPUT card ──
    {
        auto cardRect = juce::Rectangle<float>(14.0f, 372.0f, w - 28.0f, 88.0f);
        drawCard(g, cardRect, 10.0f);

        g.setFont(juce::FontOptions(8.5f).withStyle("Bold"));
        g.setColour(textDim);
        g.drawText("OUTPUT", 26, 374, 100, 16, juce::Justification::centredLeft);
    }

    // ── Input Meter ──
    auto drawMeter = [&](float x, float y, float mw, float mh, float level, const juce::String& label)
    {
        // Track
        g.setColour(juce::Colour(0xff0c0c1a));
        g.fillRoundedRectangle(x, y, mw, mh, 3.0f);

        // Level
        float db = juce::Decibels::gainToDecibels(level, -60.0f);
        float norm = juce::jmap(db, -60.0f, 0.0f, 0.0f, 1.0f);
        float fillH = mh * norm;

        juce::Colour mColor = norm > 0.9f ? red : norm > 0.6f ? amber : green;

        // Gradient fill
        g.setGradientFill(juce::ColourGradient(
            mColor.withAlpha(0.4f), x, y + mh,
            mColor, x, y + mh - fillH, false));
        g.fillRoundedRectangle(x, y + mh - fillH, mw, fillH, 3.0f);

        // Glow
        g.setColour(mColor.withAlpha(0.12f));
        g.fillRoundedRectangle(x - 2, y + mh - fillH - 2, mw + 4, fillH + 4, 4.0f);

        // Label
        g.setFont(juce::FontOptions(7.5f));
        g.setColour(textDim);
        g.drawText(label, x - 6, y + mh + 4, mw + 12, 12, juce::Justification::centred);
    };

    drawMeter(26.0f, 394.0f, 10.0f, 54.0f, inputLevel, "IN");
    drawMeter((float)w - 36.0f, 394.0f, 10.0f, 54.0f, outputLevel, "OUT");

    // ── A/B State Indicator ──
    {
        juce::Colour stateColor = abStateIsA ? accentCyan : accentPurp;
        float indicatorX = (float)w - 190.0f;

        g.setColour(stateColor.withAlpha(0.1f));
        g.fillRoundedRectangle(indicatorX, 58.0f, 50.0f, 16.0f, 8.0f);
        g.setColour(stateColor);
        g.setFont(juce::FontOptions(8.0f).withStyle("Bold"));
        g.drawText(abStateIsA ? "STATE A" : "STATE B",
            indicatorX, 58.0f, 50.0f, 16.0f, juce::Justification::centred);
    }

    // ── Bottom Bar ──
    {
        g.setColour(bgSecondary);
        g.fillRect(0, h - 24, w, 24);

        g.setFont(juce::FontOptions(8.0f));
        g.setColour(textDim);
        g.drawText("VocalChain v1.0 | Artist Vocal Processing", 16, h - 24, w - 32, 24,
            juce::Justification::centredLeft);

        // Status dot
        g.setColour(green);
        g.fillEllipse((float)w - 30.0f, (float)h - 15.0f, 6.0f, 6.0f);
    }
}

// ══════════════════════════════════════════════
//  LAYOUT
// ══════════════════════════════════════════════

void VocalChainEditor::resized()
{
    int w = getWidth();
    int margin = 28;
    int cardInner = w - 56;

    // ── Preset buttons (inside card) ──
    {
        int y = 78;
        int gap = 6;
        int btnW = (cardInner - gap * 4) / 5;
        for (int i = 0; i < presetButtons.size(); ++i)
            presetButtons[i]->setBounds(margin + i * (btnW + gap), y, btnW, 34);
    }

    // ── Voice Tuner knobs (inside card) ──
    {
        int y = 152;
        int knobSize = 68;
        int spacing = cardInner / 4;
        int offsetX = margin + (spacing - knobSize) / 2;

        brightnessSlider.setBounds(offsetX + spacing * 0, y, knobSize, knobSize);
        brightnessLabel.setBounds(offsetX + spacing * 0, y + knobSize, knobSize, 14);
        bodySlider.setBounds(offsetX + spacing * 1, y, knobSize, knobSize);
        bodyLabel.setBounds(offsetX + spacing * 1, y + knobSize, knobSize, 14);
        intensitySlider.setBounds(offsetX + spacing * 2, y, knobSize, knobSize);
        intensityLabel.setBounds(offsetX + spacing * 2, y + knobSize, knobSize, 14);
        correctionSpeedSlider.setBounds(offsetX + spacing * 3, y, knobSize, knobSize);
        correctionLabel.setBounds(offsetX + spacing * 3, y + knobSize, knobSize, 14);
    }

    // ── Module toggles grid 2×4 (inside card) ──
    {
        int y = 284;
        int colW = cardInner / 4;
        for (int i = 0; i < bypassButtons.size(); ++i)
        {
            int col = i % 4;
            int row = i / 4;
            bypassButtons[i]->setBounds(margin + col * colW + 4, y + row * 34, colW - 8, 28);
        }
    }

    // ── Output row (inside card) ──
    {
        int y = 396;
        dryWetLabel.setBounds(52, y, 62, 18);
        dryWetSlider.setBounds(114, y, w - 368, 18);

        int rightX = w - 228;
        abButton.setBounds(rightX, y, 56, 24);
        saveButton.setBounds(rightX + 62, y, 56, 24);
        loadButton.setBounds(rightX + 124, y, 56, 24);
    }
}

void VocalChainEditor::timerCallback()
{
    inputLevel = processor.getInputLevel();
    outputLevel = processor.getOutputLevel();
    repaint();
}
