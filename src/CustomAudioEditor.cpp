#include "CustomAudioEditor.h"

CustomAudioEditor::CustomAudioEditor (RNBO::JuceAudioProcessor* const p, RNBO::CoreObject& rnboObject, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (p)
    , _audioProcessor(p)
    , _rnboObject(rnboObject) // 参照メンバーを初期化（必須）
    , valueTreeState(vts)
{
    // ルック＆フィールの設定
    greyLookAndFeel.setColourScheme(juce::LookAndFeel_V4::getGreyColourScheme());
    
    //Mix
    addAndMakeVisible(dial1Slider);
    // デバッグ: vts の state を確認
    DBG("vts.state: " + valueTreeState.state.toXmlString());
    std::cout << "preLowcut" << std::endl;
    //スライダーひAPVTSのパラメータを紐づけます。
    dial1Attachment.reset (new SliderAttachment (valueTreeState, "preLowcut", dial1Slider));
    dial1Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial1Slider.setTextValueSuffix (" %");     
    dial1Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial1Slider.getTextBoxWidth(), dial1Slider.getTextBoxHeight());
    dial1Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label1);
    label1.setText ("preLowCut", juce::dontSendNotification);
    label1.setJustificationType(juce::Justification::centred);
    
    //All-pass Filter
    addAndMakeVisible(dial2Slider);
    std::cout << "preHighcut" << std::endl;
    dial2Attachment.reset (new SliderAttachment (valueTreeState, "preHighcut", dial2Slider));
    dial2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dial2Slider.setTextValueSuffix (" %");     
    dial2Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial2Slider.getTextBoxWidth(), dial2Slider.getTextBoxHeight());
    dial2Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label2);
    label2.setText ("preHighCut", juce::dontSendNotification);
    label2.setJustificationType(juce::Justification::centred);
    
    //clipType
    addAndMakeVisible(dial3Slider);
    std::cout << "clipType" << std::endl;
    dial3Attachment.reset (new SliderAttachment (valueTreeState, "clipType", dial3Slider));
    dial3Slider.setSliderStyle(juce::Slider::LinearHorizontal ); 
    //dial3Slider.setTextValueSuffix (" ms");     
    dial3Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial3Slider.getTextBoxWidth(), dial3Slider.getTextBoxHeight());
    dial3Slider.setLookAndFeel(&greyLookAndFeel);

    addAndMakeVisible(label3);
    label3.setText ("softClip", juce::dontSendNotification);
    label3.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(label4);
    label4.setText ("hardClip", juce::dontSendNotification);
    label4.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(label5);
    label5.setText ("superHardClip", juce::dontSendNotification);
    label5.setJustificationType(juce::Justification::centred);


    //clipPos
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

    //clipNeg
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

    //シンクロスイッチ
    addAndMakeVisible(syncButton);
    //dial5Slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, dial5Slider.getTextBoxWidth(), dial5Slider.getTextBoxHeight());
    syncButton.setButtonText("Pos & Neg Sync");
    syncButton.setToggleState(false, juce::dontSendNotification);
    syncButton.onClick = [this] { syncEnabled = syncButton.getToggleState(); };

    _audioProcessor->addListener(this);

    // シンクロ機能の設定
    dial4Slider.onValueChange = [this] {
        if (syncEnabled)
        //juce::dontSendNotificationフラグを使用してAudioProcessorValueTreeStateへの更新を行う。
        dial5Slider.setValue(dial4Slider.getValue(), juce::sendNotificationAsync);
    };
    
    dial5Slider.onValueChange = [this] {
        if (syncEnabled)
        dial4Slider.setValue(dial5Slider.getValue(),juce::sendNotificationAsync);
    };
    setSize(250, 500);
}

CustomAudioEditor::~CustomAudioEditor()
{
    dial1Slider.setLookAndFeel(nullptr);
    _audioProcessor->removeListener(this);
    
}

void CustomAudioEditor::paint (Graphics& g)
{
    g.fillAll( Colour::greyLevel (0.4f));
    // // タイトルなどの追加テキスト
    //g.setColour(juce::Colours::white);
    //g.setFont(15.0f);
    //g.drawFittedText("Dial Sync Example", getLocalBounds(), juce::Justification::centredTop, 1);
}

void CustomAudioEditor::resized()
{
    auto border = 6;
    auto area = getLocalBounds();
    
    
    // コンポーネントの配置
    int itemHeight = area.getHeight() / 6;
    int dial3pos = area.getWidth();
    
    auto topDialArea = area.removeFromTop(itemHeight * 2);
    dial1Slider.setBounds(topDialArea.removeFromLeft(getWidth() / 2));
    dial2Slider.setBounds(topDialArea);
    
    // スライダーの領域
    auto sliderArea = area.removeFromTop(itemHeight);
    dial3Slider.setBounds(sliderArea.reduced(20, 0).translated(0, 20)); // 少し余白を入れる

    // トグルボタンの領域
    auto toggleArea = area.removeFromTop(itemHeight);
    syncButton.setBounds(toggleArea.reduced(20, 0));
    
    // スライダーの位置とサイズを取得
    int sliderX = dial3Slider.getX();
    int sliderY = dial3Slider.getY();
    int sliderWidth = dial3Slider.getWidth();
    int labelHeight = dial3Slider.getTextBoxHeight();
    // ラベルの位置をスライダーの上に配置し、3等分
    int labelWidth = sliderWidth / 3;

    // 下部のダイアル2つ
    auto bottomDialArea = area;
    dial4Slider.setBounds(bottomDialArea.removeFromLeft(getWidth() / 2));
    dial5Slider.setBounds(bottomDialArea);

    label1.setBounds(dial1Slider.getX(), dial1Slider.getY(), dial1Slider.getWidth(),dial1Slider.getTextBoxHeight() );
    label2.setBounds(dial2Slider.getX(), dial2Slider.getY(), dial2Slider.getWidth(),dial2Slider.getTextBoxHeight() );

    label3.setBounds(sliderX , dial3Slider.getY(), labelWidth ,dial3Slider.getTextBoxHeight() );
    label4.setBounds(sliderX+ labelWidth, dial3Slider.getY(), labelWidth ,dial3Slider.getTextBoxHeight() );
    label5.setBounds(sliderX+ (labelWidth * 2), dial3Slider.getY(), labelWidth ,dial3Slider.getTextBoxHeight() );

    label6.setBounds(dial4Slider.getX(), dial4Slider.getY(), dial4Slider.getWidth(),dial4Slider.getTextBoxHeight() );
    label7.setBounds(dial5Slider.getX(), dial5Slider.getY(), dial5Slider.getWidth(),dial5Slider.getTextBoxHeight() );
}  

void CustomAudioEditor::audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float value)
{
   
}
