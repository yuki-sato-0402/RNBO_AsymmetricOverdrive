#include "CustomAudioProcessor.h"
#include "CustomAudioEditor.h"
#include <json/json.hpp>
#ifdef RNBO_INCLUDE_DESCRIPTION_FILE
#include <rnbo_description.h>
#endif

//create an instance of our custom plugin, optionally set description, presets and binary data (datarefs)
CustomAudioProcessor* CustomAudioProcessor::CreateDefault() {
	nlohmann::json patcher_desc, presets;

#ifdef RNBO_BINARY_DATA_STORAGE_NAME
	extern RNBO::BinaryDataImpl::Storage RNBO_BINARY_DATA_STORAGE_NAME;
	RNBO::BinaryDataImpl::Storage dataStorage = RNBO_BINARY_DATA_STORAGE_NAME;
#else
	RNBO::BinaryDataImpl::Storage dataStorage;
#endif
	RNBO::BinaryDataImpl data(dataStorage);

#ifdef RNBO_INCLUDE_DESCRIPTION_FILE
	patcher_desc = RNBO::patcher_description;
	presets = RNBO::patcher_presets;
#endif
  return new CustomAudioProcessor(patcher_desc, presets, data);
}

CustomAudioProcessor::CustomAudioProcessor(
    const nlohmann::json& patcher_desc,
    const nlohmann::json& presets,
    const RNBO::BinaryData& data
    ) 
  : RNBO::JuceAudioProcessor(patcher_desc, presets, data) 
{
  
  parameters = std::make_unique<juce::AudioProcessorValueTreeState>(
    *this, nullptr, juce::Identifier("APVTSTutorial"),
    juce::AudioProcessorValueTreeState::ParameterLayout {
        std::make_unique<juce::AudioParameterFloat>(ParameterID { "preLowcut",  1}, "preLowcut",
        juce::NormalisableRange<float>(0.f, 100.f, 0.01f),0.f),
        std::make_unique<juce::AudioParameterFloat>(ParameterID { "preHighcut",  1}, "preHighcut",
        juce::NormalisableRange<float>(0.f, 100.f, 0.01f),0.f),
        std::make_unique<juce::AudioParameterFloat>(ParameterID { "clipType",  1}, "clipType",
        juce::NormalisableRange<float>(0.f, 2.f, 0.01f),0.f),
        std::make_unique<juce::AudioParameterFloat>(ParameterID { "clipNeg",  1}, "clipNeg",
        juce::NormalisableRange<float>(0.f, 100.f, 0.01f),0.f),
        std::make_unique<juce::AudioParameterFloat>(ParameterID { "clipPos",  1}, "clipPos",
        juce::NormalisableRange<float>(0.f, 100.f, 0.01f),0.f)
    }
  );
  if (!parameters){
        DBG("ERROR: parameters is nullptr!");
        jassertfalse;
  }else{
        DBG("parameters successfully created!");
        DBG("parameters state: " + parameters->state.toXmlString()); // 追加
  }
 
  for (RNBO::ParameterIndex i = 0; i < rnboObject.getNumParameters(); ++i){
    RNBO::ParameterInfo info;
    rnboObject.getParameterInfo (i, &info);

    if (info.visible){
      auto paramID = juce::String (rnboObject.getParameterId (i));
      std::cout << "Parameter Index: " << i << std::endl;
      std::cout << "Min Value: " << info.min << std::endl;
      std::cout << "Max Value: " << info.max << std::endl;

      // Each apvts parameter id and range must be the same as the rnbo param object's.
      // If you hit this assertion then you need to fix the incorrect id in ParamIDs.h.
      jassert (parameters->getParameter (paramID) != nullptr);

      // If you hit these assertions then you need to fix the incorrect apvts
      // parameter range in createParameterLayout().
      jassert (info.min == parameters->getParameterRange (paramID).start);
      jassert (info.max == parameters->getParameterRange (paramID).end);
    
      if (paramID == "preLowcut") {
          // パラメータのポインタを取得
        preLowcutParameter = parameters->getRawParameterValue("preLowcut");
        parameters->addParameterListener("preLowcut", this);
        rnboObject.setParameterValue(i, *preLowcutParameter);  // RNBO に適用
      }else if(paramID == "preHighcut") {
        // パラメータのポインタを取得
        preHighcutParameter = parameters->getRawParameterValue("preHighcut");
        parameters->addParameterListener("preHighcut", this);
        rnboObject.setParameterValue(i, *preHighcutParameter);  // RNBO に適用
      }else if(paramID == "clipType"){
        clipTypeParameter = parameters->getRawParameterValue("clipType");
        parameters->addParameterListener("clipType", this);
        rnboObject.setParameterValue(i, *clipTypeParameter);  // RNBO に適用
      }else if(paramID == "clipNeg"){
        clipNegParameter = parameters->getRawParameterValue("clipNeg");
        parameters->addParameterListener("clipNeg", this);
        rnboObject.setParameterValue(i, *clipNegParameter);  // RNBO に適用
      }else{
        clipPosParameter = parameters->getRawParameterValue("clipPos");
        parameters->addParameterListener("clipPos", this);
        rnboObject.setParameterValue(i, *clipPosParameter);  // RNBO に適用
      }


    } 
  }
  
}

//processBlock() がないと JUCE のオーディオ処理が実行されない 
//JUCE のオーディオデータを RNBO に渡す, RNBO で処理された結果を JUCE に戻す,リアルタイムでパラメータを反映させる
void CustomAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
  juce::ignoreUnused (midiMessages);
  auto bufferSize = buffer.getNumSamples();

  rnboObject.prepareToProcess (getSampleRate(),static_cast<size_t> (bufferSize));

  rnboObject.process (buffer.getArrayOfWritePointers(),
                      static_cast<RNBO::Index> (buffer.getNumChannels()),
                      buffer.getArrayOfWritePointers(),
                      static_cast<RNBO::Index> (buffer.getNumChannels()),
                      static_cast<RNBO::Index> (bufferSize));
}



//このコールバック メソッドは、パラメータが変更されたときに AudioProcessorValueTreeStateによって呼び出されます。
void CustomAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    DBG("parameterChanged: " << parameterID << " = " << newValue);

    if (parameterID == "preLowcut"){
      RNBO::ParameterIndex index = rnboObject.getParameterIndexForID("preLowcut");
      rnboObject.setParameterValue(index, newValue);
    }else if(parameterID == "preHighcut"){
      RNBO::ParameterIndex index = rnboObject.getParameterIndexForID("preHighcut");
      rnboObject.setParameterValue(index, newValue);
    }else if(parameterID == "clipType"){
      RNBO::ParameterIndex index = rnboObject.getParameterIndexForID("clipType");
      rnboObject.setParameterValue(index, newValue);
    }else if(parameterID == "clipNeg"){
      RNBO::ParameterIndex index = rnboObject.getParameterIndexForID("clipNeg");
      rnboObject.setParameterValue(index, newValue);
    }else if(parameterID == "clipPos"){
      RNBO::ParameterIndex index = rnboObject.getParameterIndexForID("clipPos");
      rnboObject.setParameterValue(index, newValue);
    }
}



CustomAudioProcessor::~CustomAudioProcessor()
{
    // リスナーを必ず削除
    if (parameters != nullptr)
    {
        parameters->removeParameterListener("preLowcut", this);
        parameters->removeParameterListener("preHighcut", this);
        parameters->removeParameterListener("clipType", this);
        parameters->removeParameterListener("clipNeg", this);
        parameters->removeParameterListener("clipPos", this);
    }
}

void CustomAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void CustomAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters->state.getType()))
            parameters->replaceState(juce::ValueTree::fromXml(*xmlState));
}


AudioProcessorEditor* CustomAudioProcessor::createEditor()
{

    //AudioProcessorEditor側でAudioProcessorValueTreeStateにアクセスするための方法が必要です。
    //一般的なアプローチは、AudioProcessorからAudioProcessorValueTreeStateへの参照またはポインタを取得できるようにすること
    return new CustomAudioEditor (this, this->_rnboObject, *parameters);
    //RNBOのデフォルトエディター, 標準的なパラメータ表示, 追加のカスタマイズが限定的
    //return RNBO::JuceAudioProcessor::createEditor();
}