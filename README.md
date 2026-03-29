# RNBO_AsymmetricOverdrive
**A versatile Asymmetric Overdrive plugin (VST3 / AU / Standalone)**

This overdrive effect utilizes `tanh~` and `atan~` via RNBO to provide a wide range of distortion characteristics. By allowing positive and negative signal values to be clipped at separate thresholds, it facilitates the generation of even-order harmonics for rich, musical saturation.

## Key Features
- **Multi-Format Support**: Works as a **VST3 plugin, AU (Audio Unit) plugin, and Standalone application**.
- **Asymmetric Clipping**: Independent control over positive and negative thresholds to generate even harmonics.
- **Pre-Clipping EQ**: Built-in high-pass and low-pass filters to shape the input signal before it hits the distortion stage.
- **Wavefolding**: Redirects peaks back into the signal path once they cross the threshold, generating rich, metallic overtones and complex harmonic textures beyond standard saturation.
- **Directional Hysteresis**: Implements state-dependent distortion that reacts differently to rising and falling signals, emulating the organic "memory" and warmth of analog magnetic circuits like tape and transformers.
- **JUCE Integration**: Uses `AudioProcessorValueTreeState` for robust parameter management and DAW automation.

Built from the [rnbo.example.juce](https://github.com/Cycling74/rnbo.example.juce) template.   
The C++ source exported from RNBO is in `export/`, and the original Max patch is in `patches/`.

## Demonstration
[Youtube<img width="812" height="640" alt="Screenshot 2026-03-29 at 23 24 32" src="https://github.com/user-attachments/assets/d727217b-1541-4b56-9b4c-9379fdf80ad6" />](https://youtu.be/4reykwLRvfs)


## Parameter explanation
- **Mix** : You can control the ratio of dry sound to wet sound.

- **preLowCut** : You can to control the cutoff frequency of the pre-clipping high-pass filter.

- **preHighCut** : You can to control the cutoff frequency of the pre-clipping low-pass filter.

- **softClip ~ superHardClip** : You can change the characteristics of the distortion. Specifically, softClip uses atan~ (arctangent), hardClip uses tanh (Hyperbolic tangent), and superHard uses clip~. You can control the mix of these.

- **Pos & Neg Sync** : Check this to synchronize **clipPositiveSide** and **clipNegativeSide**.

- **clipPositiveSide** : You can control the threshold for positive clipping.

- **clipNegativeSide** : You can control the threshold for negative clipping.

- **feedCoe_up** : In hysteresis distortion, You can control the feedback coefficient applied when the input signal is rising.

- **feedCoe_down** : In hysteresis distortion, You can control the feedback coefficient applied when the input signal is falling.

- **foldMode** : Check this box to wrap the waveform around a threshold value instead of clipping.

## Reference
- [jr-granular](https://github.com/szkkng/jr-granular)  
- [Tutorial:Saving and loading your plug-in state](https://juce.com/tutorials/tutorial_audio_processor_value_tree_state/)
