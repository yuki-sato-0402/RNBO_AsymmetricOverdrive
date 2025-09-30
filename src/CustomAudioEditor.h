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

private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::LookAndFeel_V4 greyLookAndFeel;
    
    juce::Slider dial0Slider;
    juce::Slider dial1Slider;
    juce::Slider dial2Slider;
    juce::Slider dial3Slider;
    juce::Slider dial4Slider;
    juce::Slider dial5Slider;
    juce::ToggleButton syncButton; 
    
    juce::Label  label0;
    juce::Label  label1;
    juce::Label  label2;
    juce::Label  label3;
    juce::Label  label4;
    juce::Label  label5;
    juce::Label  label6;
    juce::Label  label7;
   
    std::unique_ptr<SliderAttachment> dial0Attachment;
    std::unique_ptr<SliderAttachment> dial1Attachment;
    std::unique_ptr<SliderAttachment> dial2Attachment;
    std::unique_ptr<SliderAttachment> dial3Attachment;
    std::unique_ptr<SliderAttachment> dial4Attachment;
    std::unique_ptr<SliderAttachment> dial5Attachment;
    
    bool syncEnabled = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomAudioEditor)
};
