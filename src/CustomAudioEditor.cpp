#include "CustomAudioProcessor.h"
#include "CustomAudioEditor.h"

CustomAudioEditor::CustomAudioEditor (CustomAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState(vts)// 参照メンバーを初期化（必須）
{
    greyLookAndFeel.setColourScheme(juce::LookAndFeel_V4::getGreyColourScheme());

    addAndMakeVisible(dial0Slider);
    dial0Attachment.reset (new SliderAttachment (valueTreeState, "Mix", dial0Slider));
    dial0Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial0Slider.setTextValueSuffix (" %");     
    dial0Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial0Slider.getTextBoxWidth(), dial0Slider.getTextBoxHeight());
    dial0Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label0);
    label0.setText ("Mix", juce::dontSendNotification);
    label0.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(dial1Slider);
    dial1Attachment.reset (new SliderAttachment (valueTreeState, "preLowcut", dial1Slider));
    dial1Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial1Slider.setTextValueSuffix (" %");     
    dial1Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial1Slider.getTextBoxWidth(), dial1Slider.getTextBoxHeight());
    dial1Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label1);
    label1.setText ("preLowCut", juce::dontSendNotification);
    label1.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(dial2Slider);
    dial2Attachment.reset (new SliderAttachment (valueTreeState, "preHighcut", dial2Slider));
    dial2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial2Slider.setTextValueSuffix (" %");     
    dial2Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial2Slider.getTextBoxWidth(), dial2Slider.getTextBoxHeight());
    dial2Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label2);
    label2.setText ("preHighCut", juce::dontSendNotification);
    label2.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(dial3Slider);
    dial3Attachment.reset (new SliderAttachment (valueTreeState, "clipType", dial3Slider));
    dial3Slider.setSliderStyle(juce::Slider::LinearHorizontal ); 
    dial3Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial3Slider.getTextBoxWidth(), dial3Slider.getTextBoxHeight());
    dial3Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label3);
    label3.setText ("softClip", juce::dontSendNotification);
    label3.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(label4);
    label4.setText ("hardClip", juce::dontSendNotification);
    label4.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(label5);
    label5.setText ("superHard\nClip", juce::dontSendNotification);
    label5.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(dial4Slider);
    std::cout << "clipPos" << std::endl;
    dial4Attachment.reset (new SliderAttachment (valueTreeState, "clipPos", dial4Slider));
    dial4Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial4Slider.setTextValueSuffix (" %");     
    dial4Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial4Slider.getTextBoxWidth(), dial4Slider.getTextBoxHeight());
    dial4Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label6);
    label6.setText ("clipPositiveSide", juce::dontSendNotification);
    label6.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(dial5Slider);
    std::cout << "clipNeg" << std::endl;
    dial5Attachment.reset (new SliderAttachment (valueTreeState, "clipNeg", dial5Slider));
    dial5Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial5Slider.setTextValueSuffix (" %");     
    dial5Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial5Slider.getTextBoxWidth(), dial5Slider.getTextBoxHeight());
    dial5Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label7);
    label7.setText ("clipNegativeSide", juce::dontSendNotification);
    label7.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(syncButton);
    syncButton.setButtonText("Pos & Neg Sync");
    syncButton.setToggleState(false, juce::dontSendNotification);
    syncButton.onClick = [this] { syncEnabled = syncButton.getToggleState(); };


    dial4Slider.onValueChange = [this] {
        if (syncEnabled)
        dial5Slider.setValue(dial4Slider.getValue(), juce::sendNotificationAsync);
    };
    
    dial5Slider.onValueChange = [this] {
        if (syncEnabled)
        dial4Slider.setValue(dial5Slider.getValue(),juce::sendNotificationAsync);
    };
    setSize(250, 700);
}

void CustomAudioEditor::paint (Graphics& g)
{
   // Get the background color from the midnight scheme
   auto backgroundColour = juce::LookAndFeel_V4::getGreyColourScheme().getUIColour(juce::LookAndFeel_V4::ColourScheme::UIColour::windowBackground);

   // Fill the entire component with this color
   g.fillAll(backgroundColour);
}

void CustomAudioEditor::resized()
{
    auto area = getLocalBounds();
    const int componentWidth1 = (area.getWidth() - 30) / 2;
    const int componentHeight = area.getHeight() / 8;
    const int padding = 10;        
    
    dial0Slider.setBounds(padding, 0 ,  componentWidth1 , componentHeight * 2);

    dial1Slider.setBounds(padding, dial0Slider.getBottom(),  componentWidth1 , componentHeight * 2);
    dial2Slider.setBounds(dial1Slider.getRight() + padding, dial0Slider.getBottom() , componentWidth1, componentHeight * 2);
    
    dial3Slider.setBounds(padding, dial2Slider.getBottom() + 20,  componentWidth1 * 2 , componentHeight );

    syncButton.setBounds(padding, dial3Slider.getBottom()-10,  componentWidth1 * 2 , componentHeight );

    dial4Slider.setBounds(padding, syncButton.getBottom() -20,  componentWidth1 , componentHeight * 2);
    dial5Slider.setBounds(dial1Slider.getRight() + padding, syncButton.getBottom()-20 , componentWidth1, componentHeight * 2);
    
    
    int sliderX = dial3Slider.getX();
    int labelWidth = dial3Slider.getWidth() / 3;

    label0.setBounds(dial0Slider.getX(), dial0Slider.getY()+10 , dial0Slider.getWidth(),dial0Slider.getTextBoxHeight() );
    label1.setBounds(dial1Slider.getX(), dial1Slider.getY()+10 , dial1Slider.getWidth(),dial1Slider.getTextBoxHeight() );
    label2.setBounds(dial2Slider.getX(), dial2Slider.getY()+10 , dial2Slider.getWidth(),dial2Slider.getTextBoxHeight() );

    label3.setBounds(sliderX , dial3Slider.getY(), labelWidth ,dial3Slider.getTextBoxHeight() );
    label4.setBounds(sliderX+ labelWidth, dial3Slider.getY(), labelWidth ,dial3Slider.getTextBoxHeight() );
    label5.setBounds(sliderX+ (labelWidth * 2), dial3Slider.getY() - 15 , labelWidth + 10 ,dial3Slider.getTextBoxHeight() * 2);

    label6.setBounds(dial4Slider.getX(), dial4Slider.getY()+10, dial4Slider.getWidth(),dial4Slider.getTextBoxHeight() );
    label7.setBounds(dial5Slider.getX(), dial5Slider.getY()+10, dial5Slider.getWidth(),dial5Slider.getTextBoxHeight() );
}  
