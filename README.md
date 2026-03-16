# 🎛️ VocalChain VST

![Format](https://img.shields.io/badge/plugin-VST3-blue.svg)
![Framework](https://img.shields.io/badge/framework-JUCE-orange.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%2064--bit-lightgrey.svg)

VocalChain is a beautifully designed, all-in-one vocal processing VST3 plugin that instantly gives you producer-quality vocals. Modeled after the signature mix chains of industry-leading artists, VocalChain allows you to dial in professional vocals with a single click, all from a gorgeous glassmorphism UI.

---

<p align="center">
  <!-- TODO: Replace with actual screenshot paths -->
  <img src="https://via.placeholder.com/800x560.png?text=VocalChain+VST+Editor" width="800" />
</p>

## ✨ Features

- **🎤 Artist Signature Presets:** 
  Includes out-of-the-box, fine-tuned presets modeled after today's top artists:
  - 🔴 **Yeat:** Aggressive saturation, rapid pitch correction, and tight slap delay.
  - 🟣 **Drake:** Smooth EQ, lush reverb, and gentle de-essing.
  - 🟡 **Juice WRLD:** Emotional, wide delays combined with distinct vocal tuning.
  - ⚪ **Kanye:** Unique harmonic excitation and heavy compression.
  - 🔵 **Uzi:** Bright, energetic, and heavily colored.
- **🎚️ 8-Stage processing modules:** Includes Gate, Pitch Correction, EQ, Compressor, De-Esser, Saturation, Delay, and Reverb. Toggle them individually!
- **⚡ Voice Tuner Macros:** Four intuitive macro knobs (Brightness, Body, Intensity, Correction) mapping to complex underlying DSP parameters to seamlessly tune your vocal instantly.
- **🎨 Premium Dark UI:** 800x560 modern glassmorphism GUI, gradient metering, A/B state comparisons, and smooth parameter animations.
- **💾 Preset Management:** Save your own customized vocal chains as `.vcpreset` files and load them into any project.

## 💾 Installation

You don't need to manually configure source files or compile anything to use it in your DAW! 

1. Run `VocalChain-Setup-v1.0.0.exe`.
2. The installer automatically places `VocalChain.vst3` into your standard Windows VST3 directory (`C:\Program Files\Common Files\VST3`).
3. Open your DAW (FL Studio, Ableton, Cubase, etc.) and run a plugin scan.
4. Drag onto your vocal track and create!

*(Note: VocalChain is a 64-bit VST3 plugin. Ensure your DAW is running in 64-bit mode).*

## 🛠️ Building from Source

This plugin is built using the [JUCE framework](https://juce.com/).

### Prerequisites
- Visual Studio 2022 (with Desktop Development with C++)
- CMake 3.22+

### Quick Build (Windows)
```cmd
cd VocalChain-VST
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```
Your compiled `VocalChain.vst3` will be located in `build/VocalChain_artefacts/Release/VST3/`.

---

*Get the platinum vocal sound in seconds.*
