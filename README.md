# RNBO_AsymmetricOverdrive
**A versatile Asymmetric Overdrive plugin (VST3 / AU / Standalone)**

This overdrive effect utilizes `tanh~` and `atan~` via RNBO to provide a wide range of distortion characteristics. By allowing positive and negative signal values to be clipped at separate thresholds, it facilitates the generation of even-order harmonics for rich, musical saturation.

## Key Features
- **Multi-Format Support**: Works as a **VST3 plugin, AU (Audio Unit) plugin, and Standalone application**.
- **Asymmetric Clipping**: Independent control over positive and negative thresholds to generate even harmonics.
- **Pre-Clipping EQ**: Built-in high-pass and low-pass filters to shape the input signal before it hits the distortion stage.
- **JUCE Integration**: Uses `AudioProcessorValueTreeState` for robust parameter management and DAW automation.

Built from the [rnbo.example.juce](https://github.com/Cycling74/rnbo.example.juce) template.   
The C++ source exported from RNBO is in `export/`, and the original Max patch is in `patches/`.

## Demonstration
[Youtube<img width="903" alt="overdrive" src="https://github.com/user-attachments/assets/5abd93a9-05c6-4c67-80a3-7d5f351979f4" />](https://youtu.be/sKyrQdIMzz8)

## Parameter explanation
- **Mix** : You can control the ratio of dry sound to wet sound.

- **preLowCut** : You can to control the cutoff frequency of the pre-clipping high-pass filter.

- **preHighCut** : You can to control the cutoff frequency of the pre-clipping low-pass filter.

- **softClip ~ superHardClip** : You can change the characteristics of the distortion. Specifically, softClip uses atan~ (arctangent), hardClip uses tanh (Hyperbolic tangent), and superHard uses clip~. You can control the mix of these.

- **Pos & Neg Sync** : Check this to synchronize **clipPositiveSide** and **clipNegativeSide**.

- **clipPositiveSide** : You can control the threshold for positive clipping.

- **clipNegativeSide** : You can control the threshold for negative clipping.

## Reference
- [jr-granular](https://github.com/szkkng/jr-granular)  
- [Tutorial:Saving and loading your plug-in state](https://juce.com/tutorials/tutorial_audio_processor_value_tree_state/)
