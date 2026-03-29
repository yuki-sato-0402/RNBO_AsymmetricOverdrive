#include "CustomAudioProcessor.h"
#include "CustomAudioEditor.h"

CustomAudioEditor::CustomAudioEditor (CustomAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState(vts), audioProcessor(p)
{
    addAndMakeVisible(audioProcessor.visualizer);
    audioProcessor.visualizer.setColours(juce::Colours::darkred, juce::Colours::white);

    addAndMakeVisible(mixSlider);
    mixSliderAttachment.reset (new SliderAttachment (valueTreeState, "Mix", mixSlider));
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextValueSuffix (" %");     
    mixSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, mixSlider.getTextBoxWidth(), mixSlider.getTextBoxHeight());
    mixSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    mixSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    mixSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);

    addAndMakeVisible(mixLabel);
    mixLabel.setText ("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(lowCutSlider);
    lowCutSliderAttachment.reset (new SliderAttachment (valueTreeState, "preLowcut", lowCutSlider));
    lowCutSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowCutSlider.setTextValueSuffix (" %");     
    lowCutSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, lowCutSlider.getTextBoxWidth(), lowCutSlider.getTextBoxHeight());
    lowCutSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    lowCutSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    lowCutSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    lowCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    lowCutSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);

    addAndMakeVisible(lowCutLabel);
    lowCutLabel.setText ("preLowCut", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(highCutSlider);
    highCutSliderAttachment.reset (new SliderAttachment (valueTreeState, "preHighcut", highCutSlider));
    highCutSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    highCutSlider.setTextValueSuffix (" %");     
    highCutSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, highCutSlider.getTextBoxWidth(), highCutSlider.getTextBoxHeight());
    highCutSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    highCutSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    highCutSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    highCutSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    highCutSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);

    addAndMakeVisible(highCutLabel);
    highCutLabel.setText ("preHighCut", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(clipTypeSlider);
    clipTypeAttachment.reset (new SliderAttachment (valueTreeState, "clipType", clipTypeSlider));
    clipTypeSlider.setSliderStyle(juce::Slider::LinearHorizontal ); 
    clipTypeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, clipTypeSlider.getTextBoxWidth(), clipTypeSlider.getTextBoxHeight());
    clipTypeSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::white);
    clipTypeSlider.setColour(juce::Slider::trackColourId, juce::Colours::darkred.withAlpha(0.75f));
    clipTypeSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    clipTypeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    clipTypeSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);


    addAndMakeVisible(softClipLabel);
    softClipLabel.setText ("softClip", juce::dontSendNotification);
    softClipLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(hardClipLabel);
    hardClipLabel.setText ("hardClip", juce::dontSendNotification);
    hardClipLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(superHardClipLabel);
    superHardClipLabel.setText ("superHard\nClip", juce::dontSendNotification);
    superHardClipLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(clipPosSlider);
    clipPosSliderAttachment.reset (new SliderAttachment (valueTreeState, "clipPos", clipPosSlider));
    clipPosSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    clipPosSlider.setTextValueSuffix (" %");     
    clipPosSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, clipPosSlider.getTextBoxWidth(), clipPosSlider.getTextBoxHeight());
    clipPosSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    clipPosSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    clipPosSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    clipPosSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    clipPosSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);


    addAndMakeVisible(clipPosLabel);
    clipPosLabel.setText ("clipPositiveSide", juce::dontSendNotification);
    clipPosLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(clipNegSlider);
    clipNegSliderAttachment.reset (new SliderAttachment (valueTreeState, "clipNeg", clipNegSlider));
    clipNegSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    clipNegSlider.setTextValueSuffix (" %");     
    clipNegSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, clipNegSlider.getTextBoxWidth(), clipNegSlider.getTextBoxHeight());
    clipNegSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    clipNegSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    clipNegSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    clipNegSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    clipNegSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);


    addAndMakeVisible(clipNegLabel);
    clipNegLabel.setText ("clipNegativeSide", juce::dontSendNotification);
    clipNegLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(syncButton);
    syncButton.setButtonText("Pos & Neg Sync");
    syncButton.setToggleState(false, juce::dontSendNotification);
    syncButton.onClick = [this] { syncEnabled = syncButton.getToggleState(); };

    clipPosSlider.onValueChange = [this] {
        if (syncEnabled)
        clipNegSlider.setValue(clipPosSlider.getValue(), juce::sendNotificationAsync);
    };
    
    clipNegSlider.onValueChange = [this] {
        if (syncEnabled)
        clipPosSlider.setValue(clipNegSlider.getValue(),juce::sendNotificationAsync);
    };

    addAndMakeVisible(feedCoe_upSlider);
    feedCoe_upSliderAttachment.reset (new SliderAttachment (valueTreeState, "feedCoe_up", feedCoe_upSlider));
    feedCoe_upSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedCoe_upSlider.setTextValueSuffix (" %");     
    feedCoe_upSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, feedCoe_upSlider.getTextBoxWidth(), feedCoe_upSlider.getTextBoxHeight());
    feedCoe_upSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    feedCoe_upSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    feedCoe_upSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    feedCoe_upSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    feedCoe_upSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);

    addAndMakeVisible(feedCoe_upLabel);
    feedCoe_upLabel.setText ("feedCoe_up", juce::dontSendNotification);
    feedCoe_upLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(feedCoe_downSlider);
    feedCoe_downSliderAttachment.reset (new SliderAttachment (valueTreeState, "feedCoe_down", feedCoe_downSlider));
    feedCoe_downSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedCoe_downSlider.setTextValueSuffix (" %");     
    feedCoe_downSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, feedCoe_downSlider.getTextBoxWidth(), feedCoe_downSlider.getTextBoxHeight());
    feedCoe_downSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    feedCoe_downSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkred.withAlpha(0.75f));
    feedCoe_downSlider.setColour(juce::Slider::thumbColourId , juce::Colours::darkred.brighter(1.5));
    feedCoe_downSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    feedCoe_downSlider.setColour(juce::Slider::textBoxOutlineColourId , juce::Colours::white);

    addAndMakeVisible(feedCoe_downLabel);
    feedCoe_downLabel.setText ("feedCoe_up", juce::dontSendNotification);
    feedCoe_downLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(foldModeButton);
    foldModeButton.setButtonText("foldMode");
    foldModeButtonAttachment.reset (new ButtonAttachment (valueTreeState, "foldMode", foldModeButton));
   
    setSize(700, 500);
}

void CustomAudioEditor::paint (Graphics& g)
{
   g.fillAll(juce::Colours::darkred);
}

void CustomAudioEditor::resized()
{
    auto area = getLocalBounds();
    const int componentWidth = (area.getWidth() - 100) / 4;
    const int componentHeight = (area.getHeight() - 80) / 3;
    const int padding = 20;        
    


    lowCutSlider.setBounds(padding, padding * 2,  componentWidth , componentHeight - padding);
    highCutSlider.setBounds(lowCutSlider.getRight() + padding, padding * 2, componentWidth, componentHeight - padding);
    audioProcessor.visualizer.setBounds(highCutSlider.getRight() + padding, padding,  componentWidth * 2 , componentHeight);
    
    clipTypeSlider.setBounds(padding, highCutSlider.getBottom() + padding * 2,  componentWidth * 2 , componentHeight / 2 - padding);
    mixSlider.setBounds(clipTypeSlider.getRight() + padding * 2, highCutSlider.getBottom() + padding * 2,  componentWidth , componentHeight- padding);

    syncButton.setBounds(padding, clipTypeSlider.getBottom() ,  componentWidth, componentHeight / 2);
    foldModeButton.setBounds(mixSlider.getRight() + padding, clipTypeSlider.getBottom(),  componentWidth, componentHeight / 2);

    clipPosSlider.setBounds(padding, syncButton.getBottom() + padding * 2,  componentWidth , componentHeight- padding);
    clipNegSlider.setBounds(lowCutSlider.getRight() + padding, syncButton.getBottom()+ padding * 2, componentWidth, componentHeight- padding);
    feedCoe_upSlider.setBounds(clipNegSlider.getRight() + padding, syncButton.getBottom() + padding * 2,  componentWidth , componentHeight- padding);
    feedCoe_downSlider.setBounds(feedCoe_upSlider.getRight() + padding, syncButton.getBottom() + padding * 2,  componentWidth , componentHeight- padding);
    
    
    int sliderX = clipTypeSlider.getX();
    int labelWidth = clipTypeSlider.getWidth() / 3;

    mixLabel.setBounds(mixSlider.getX(), mixSlider.getY() - padding , mixSlider.getWidth(),mixSlider.getTextBoxHeight() );
    lowCutLabel.setBounds(lowCutSlider.getX(), lowCutSlider.getY()- padding, lowCutSlider.getWidth(),lowCutSlider.getTextBoxHeight() );
    highCutLabel.setBounds(highCutSlider.getX(), highCutSlider.getY()- padding, highCutSlider.getWidth(),highCutSlider.getTextBoxHeight() );

    softClipLabel.setBounds(sliderX , clipTypeSlider.getY()- padding, labelWidth ,clipTypeSlider.getTextBoxHeight() );
    hardClipLabel.setBounds(sliderX+ labelWidth, clipTypeSlider.getY()- padding, labelWidth ,clipTypeSlider.getTextBoxHeight() );
    superHardClipLabel.setBounds(sliderX+ (labelWidth * 2), clipTypeSlider.getY() - padding - 10, labelWidth + 10 ,clipTypeSlider.getTextBoxHeight() * 2);

    clipPosLabel.setBounds(clipPosSlider.getX(), clipPosSlider.getY()- padding, clipPosSlider.getWidth(),clipPosSlider.getTextBoxHeight() );
    clipNegLabel.setBounds(clipNegSlider.getX(), clipNegSlider.getY()- padding, clipNegSlider.getWidth(),clipNegSlider.getTextBoxHeight() );
    feedCoe_upLabel.setBounds(feedCoe_upSlider.getX(), feedCoe_upSlider.getY()- padding, feedCoe_upSlider.getWidth(),feedCoe_upSlider.getTextBoxHeight() );
    feedCoe_downLabel.setBounds(feedCoe_downSlider.getX(), feedCoe_downSlider.getY()- padding, feedCoe_downSlider.getWidth(),feedCoe_downSlider.getTextBoxHeight() );
}  
