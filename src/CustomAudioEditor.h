#include "JuceHeader.h"
#include "RNBO.h"

class CustomAudioEditor : public juce::AudioProcessorEditor
{
public:
    CustomAudioEditor(CustomAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~CustomAudioEditor() override = default;
    void paint (Graphics& g) override;
    void resized() override; 
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    
private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    CustomAudioProcessor& audioProcessor;
    
    juce::Slider mixSlider;
    juce::Slider lowCutSlider;
    juce::Slider highCutSlider;
    juce::Slider clipTypeSlider;
    juce::Slider clipPosSlider;
    juce::Slider clipNegSlider;
    juce::ToggleButton syncButton; 
    juce::Slider feedCoe_upSlider;
    juce::Slider feedCoe_downSlider;
    juce::ToggleButton foldModeButton;

    juce::Label  mixLabel;
    juce::Label  lowCutLabel;
    juce::Label  highCutLabel;
    juce::Label  softClipLabel;
    juce::Label  hardClipLabel;
    juce::Label  superHardClipLabel;
    juce::Label  clipPosLabel;
    juce::Label  clipNegLabel;
    juce::Label  feedCoe_upLabel;
    juce::Label  feedCoe_downLabel;
   
    std::unique_ptr<SliderAttachment> mixSliderAttachment;
    std::unique_ptr<SliderAttachment> lowCutSliderAttachment;
    std::unique_ptr<SliderAttachment> highCutSliderAttachment;
    std::unique_ptr<SliderAttachment> clipTypeAttachment;
    std::unique_ptr<SliderAttachment> clipPosSliderAttachment;
    std::unique_ptr<SliderAttachment> clipNegSliderAttachment;
    std::unique_ptr<SliderAttachment> feedCoe_upSliderAttachment;
    std::unique_ptr<SliderAttachment> feedCoe_downSliderAttachment;
    std::unique_ptr<ButtonAttachment> foldModeButtonAttachment;
    
    bool syncEnabled = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomAudioEditor)
};
