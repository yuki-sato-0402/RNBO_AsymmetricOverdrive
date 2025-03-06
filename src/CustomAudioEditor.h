#include "JuceHeader.h"
#include "RNBO.h"
#include "RNBO_JuceAudioProcessor.h"
//#include "CustomAudioProcessor.h"

class CustomAudioEditor : public AudioProcessorEditor, private AudioProcessorListener 
{
public:
    CustomAudioEditor(RNBO::JuceAudioProcessor* const p, RNBO::CoreObject& rnboObject,juce::AudioProcessorValueTreeState& vts);
    ~CustomAudioEditor() override;
    void paint (Graphics& g) override;
    void resized() override; 
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

private:
///コンストラクタでプロセッサ側から受け取るAPVTSの参照を格納するメンバを定義,パラメータとUIを紐づけるため。
    juce::AudioProcessorValueTreeState& valueTreeState; // ✅ 参照で保持

    // LookAndFeel クラス
    juce::LookAndFeel_V4 greyLookAndFeel;
    
    juce::Slider dial0Slider;
    juce::Slider dial1Slider;
    juce::Slider dial2Slider;
    juce::Slider dial3Slider;
    juce::Slider dial4Slider;
    juce::Slider dial5Slider;
    juce::ToggleButton syncButton; // シンクロスイッチ
    
    juce::Label  label0;
    juce::Label  label1;
    juce::Label  label2;
    juce::Label  label3;
    juce::Label  label4;
    juce::Label  label5;
    juce::Label  label6;
    juce::Label  label7;
   
    //AudioProcessorValueTreeState::SliderAttachmentのスマートポインタ
    std::unique_ptr<SliderAttachment> dial0Attachment;
    std::unique_ptr<SliderAttachment> dial1Attachment;
    std::unique_ptr<SliderAttachment> dial2Attachment;
    std::unique_ptr<SliderAttachment> dial3Attachment;
    std::unique_ptr<SliderAttachment> dial4Attachment;
    std::unique_ptr<SliderAttachment> dial5Attachment;
    
    //CustomAudioProcessor& processor;
    //JUCEの AudioProcessorListener インターフェースのメソッドをオーバーライドしたもので、
    //オーディオプロセッサーに関する変更を通知するために使用されるもの
    void audioProcessorChanged (AudioProcessor*, const ChangeDetails&) override { }
    void audioProcessorParameterChanged(AudioProcessor*, int parameterIndex, float) override;
    bool syncEnabled = false;

protected:
    AudioProcessor                              *_audioProcessor;
    RNBO::CoreObject&                           _rnboObject;  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomAudioEditor)
};
