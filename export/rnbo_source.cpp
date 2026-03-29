/*******************************************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER $200k in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below $200k annual revenue or funding.

For entities with OVER $200k in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing@cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
*******************************************************************************************************************/

#ifdef RNBO_LIB_PREFIX
#define STR_IMPL(A) #A
#define STR(A) STR_IMPL(A)
#define RNBO_LIB_INCLUDE(X) STR(RNBO_LIB_PREFIX/X)
#else
#define RNBO_LIB_INCLUDE(X) #X
#endif // RNBO_LIB_PREFIX
#ifdef RNBO_INJECTPLATFORM
#define RNBO_USECUSTOMPLATFORM
#include RNBO_INJECTPLATFORM
#endif // RNBO_INJECTPLATFORM

#include RNBO_LIB_INCLUDE(RNBO_Common.h)
#include RNBO_LIB_INCLUDE(RNBO_AudioSignal.h)

namespace RNBO {


#define trunc(x) ((Int)(x))
#define autoref auto&

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

template <class ENGINE = INTERNALENGINE> class rnbomatic : public PatcherInterfaceImpl {

friend class EngineCore;
friend class Engine;
friend class MinimalEngine<>;
public:

rnbomatic()
: _internalEngine(this)
{
}

~rnbomatic()
{
    deallocateSignals();
}

Index getNumMidiInputPorts() const {
    return 1;
}

void processMidiEvent(MillisecondTime time, int port, ConstByteArray data, Index length) {
    this->updateTime(time, (ENGINE*)nullptr);
    this->ctlin_01_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_02_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_03_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_04_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_05_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_06_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_07_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_08_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_09_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    const SampleValue * const* inputs,
    Index numInputs,
    SampleValue * const* outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime(), (ENGINE*)nullptr, true);
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    this->p_01_perform(in1, in2, this->signals[0], this->signals[1], n);

    this->gen_01_perform(
        this->signals[0],
        this->signals[1],
        this->gen_01_clipPos,
        this->gen_01_clipNeg,
        this->gen_01_clipType,
        this->gen_01_feedCoe_up,
        this->gen_01_feedCoe_down,
        this->gen_01_foldMode,
        this->gen_01_Mix,
        out1,
        out2,
        n
    );

    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->advanceTime((ENGINE*)nullptr);
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    RNBO_ASSERT(this->_isInitialized);

    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 2; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 0;
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->p_01->processDataViewUpdate(index, time);
}

void initialize() {
    RNBO_ASSERT(!this->_isInitialized);
    this->assign_defaults();
    this->applyState();
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
    this->_isInitialized = true;
}

void getPreset(PatcherStateInterface& preset) {
    this->updateTime(this->getEngine()->getCurrentTime(), (ENGINE*)nullptr);
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "preLowcut"));
    this->param_02_getPresetValue(getSubState(preset, "preHighcut"));
    this->param_03_getPresetValue(getSubState(preset, "foldMode"));
    this->param_04_getPresetValue(getSubState(preset, "feedCoe_down"));
    this->param_05_getPresetValue(getSubState(preset, "feedCoe_up"));
    this->param_06_getPresetValue(getSubState(preset, "Mix"));
    this->param_07_getPresetValue(getSubState(preset, "clipType"));
    this->param_08_getPresetValue(getSubState(preset, "clipNeg"));
    this->param_09_getPresetValue(getSubState(preset, "clipPos"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "Pre-EQ~"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time, (ENGINE*)nullptr);
    this->p_01->setPreset(time, getSubState(getSubState(preset, "__sps"), "Pre-EQ~"));
    this->param_01_setPresetValue(getSubState(preset, "preLowcut"));
    this->param_02_setPresetValue(getSubState(preset, "preHighcut"));
    this->param_03_setPresetValue(getSubState(preset, "foldMode"));
    this->param_04_setPresetValue(getSubState(preset, "feedCoe_down"));
    this->param_05_setPresetValue(getSubState(preset, "feedCoe_up"));
    this->param_06_setPresetValue(getSubState(preset, "Mix"));
    this->param_07_setPresetValue(getSubState(preset, "clipType"));
    this->param_08_setPresetValue(getSubState(preset, "clipNeg"));
    this->param_09_setPresetValue(getSubState(preset, "clipPos"));
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time, (ENGINE*)nullptr);

    switch (index) {
    case 0:
        {
        this->param_01_value_set(v);
        break;
        }
    case 1:
        {
        this->param_02_value_set(v);
        break;
        }
    case 2:
        {
        this->param_03_value_set(v);
        break;
        }
    case 3:
        {
        this->param_04_value_set(v);
        break;
        }
    case 4:
        {
        this->param_05_value_set(v);
        break;
        }
    case 5:
        {
        this->param_06_value_set(v);
        break;
        }
    case 6:
        {
        this->param_07_value_set(v);
        break;
        }
    case 7:
        {
        this->param_08_value_set(v);
        break;
        }
    case 8:
        {
        this->param_09_value_set(v);
        break;
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        break;
        }
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processParameterBangEvent(ParameterIndex index, MillisecondTime time) {
    this->setParameterValue(index, this->getParameterValue(index), time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        {
        return this->param_01_value;
        }
    case 1:
        {
        return this->param_02_value;
        }
    case 2:
        {
        return this->param_03_value;
        }
    case 3:
        {
        return this->param_04_value;
        }
    case 4:
        {
        return this->param_05_value;
        }
    case 5:
        {
        return this->param_06_value;
        }
    case 6:
        {
        return this->param_07_value;
        }
    case 7:
        {
        return this->param_08_value;
        }
    case 8:
        {
        return this->param_09_value;
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        return 0;
        }
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 9 + this->p_01->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "preLowcut";
        }
    case 1:
        {
        return "preHighcut";
        }
    case 2:
        {
        return "foldMode";
        }
    case 3:
        {
        return "feedCoe_down";
        }
    case 4:
        {
        return "feedCoe_up";
        }
    case 5:
        {
        return "Mix";
        }
    case 6:
        {
        return "clipType";
        }
    case 7:
        {
        return "clipNeg";
        }
    case 8:
        {
        return "clipPos";
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "preLowcut";
        }
    case 1:
        {
        return "preHighcut";
        }
    case 2:
        {
        return "foldMode";
        }
    case 3:
        {
        return "feedCoe_down";
        }
    case 4:
        {
        return "feedCoe_up";
        }
    case 5:
        {
        return "Mix";
        }
    case 6:
        {
        return "clipType";
        }
    case 7:
        {
        return "clipNeg";
        }
    case 8:
        {
        return "clipPos";
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        return "bogus";
        }
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 2:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0.5;
            info->min = 0;
            info->max = 0.99;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0.5;
            info->min = 0;
            info->max = 0.99;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 5:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 6:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 2;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 7:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 8:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        default:
            {
            index -= 9;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            break;
            }
        }
    }
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 2:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
        }
    case 6:
        {
        {
            value = (value < 0 ? 0 : (value > 2 ? 2 : value));
            ParameterValue normalizedValue = (value - 0) / (2 - 0);
            return normalizedValue;
        }
        }
    case 0:
    case 1:
    case 5:
    case 7:
    case 8:
        {
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
        }
    case 3:
    case 4:
        {
        {
            value = (value < 0 ? 0 : (value > 0.99 ? 0.99 : value));
            ParameterValue normalizedValue = (value - 0) / (0.99 - 0);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 2:
        {
        {
            {
                return 0 + value * (1 - 0);
            }
        }
        }
    case 6:
        {
        {
            {
                return 0 + value * (2 - 0);
            }
        }
        }
    case 0:
    case 1:
    case 5:
    case 7:
    case 8:
        {
        {
            {
                return 0 + value * (100 - 0);
            }
        }
        }
    case 3:
    case 4:
        {
        {
            {
                return 0 + value * (0.99 - 0);
            }
        }
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        return this->param_01_value_constrain(value);
        }
    case 1:
        {
        return this->param_02_value_constrain(value);
        }
    case 2:
        {
        return this->param_03_value_constrain(value);
        }
    case 3:
        {
        return this->param_04_value_constrain(value);
        }
    case 4:
        {
        return this->param_05_value_constrain(value);
        }
    case 5:
        {
        return this->param_06_value_constrain(value);
        }
    case 6:
        {
        return this->param_07_value_constrain(value);
        }
    case 7:
        {
        return this->param_08_value_constrain(value);
        }
    case 8:
        {
        return this->param_09_value_constrain(value);
        }
    default:
        {
        index -= 9;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        return value;
        }
    }
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    RNBO_UNUSED(objectId);
    this->updateTime(time, (ENGINE*)nullptr);
    this->p_01->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    RNBO_UNUSED(objectId);
    this->updateTime(time, (ENGINE*)nullptr);
    this->p_01->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time, (ENGINE*)nullptr);
    this->p_01->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

class RNBOSubpatcher_04 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_04()
    {}
    
    ~RNBOSubpatcher_04()
    {
        deallocateSignals();
    }
    
    Index getNumMidiInputPorts() const {
        return 0;
    }
    
    void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        const SampleValue * const* inputs,
        Index numInputs,
        SampleValue * const* outputs,
        Index numOutputs,
        Index n
    ) {
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime(), (ENGINE*)nullptr, true);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        this->linetilde_01_perform(this->signals[0], n);
        this->onepole_tilde_02_perform(in1, this->signals[0], this->signals[1], n);
        this->dspexpr_01_perform(in1, this->signals[1], this->signals[2], n);
        this->onepole_tilde_04_perform(in2, this->signals[0], this->signals[1], n);
        this->dspexpr_02_perform(in2, this->signals[1], this->signals[0], n);
        this->linetilde_02_perform(this->signals[1], n);
        this->onepole_tilde_03_perform(this->signals[0], this->signals[1], out2, n);
        this->onepole_tilde_01_perform(this->signals[2], this->signals[1], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        RNBO_ASSERT(this->_isInitialized);
    
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 3; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->onepole_tilde_02_dspsetup(forceDSPSetup);
        this->onepole_tilde_04_dspsetup(forceDSPSetup);
        this->onepole_tilde_03_dspsetup(forceDSPSetup);
        this->onepole_tilde_01_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getNumInputChannels() const {
        return 2;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void getPreset(PatcherStateInterface& ) {}
    
    void setPreset(MillisecondTime , PatcherStateInterface& ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processParameterBangEvent(ParameterIndex index, MillisecondTime time) {
        this->setParameterValue(index, this->getParameterValue(index), time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time, (ENGINE*)nullptr);
    
        switch (tag) {
        case TAG("valin"):
            {
            if (TAG("Pre-EQ~/number_obj-11") == objectId)
                this->numberobj_01_valin_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-3") == objectId)
                this->numberobj_02_valin_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-29") == objectId)
                this->numberobj_03_valin_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-33") == objectId)
                this->numberobj_04_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Pre-EQ~/number_obj-11") == objectId)
                this->numberobj_01_format_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-3") == objectId)
                this->numberobj_02_format_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-29") == objectId)
                this->numberobj_03_format_set(payload);
    
            if (TAG("Pre-EQ~/number_obj-33") == objectId)
                this->numberobj_04_format_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Pre-EQ~/number_obj-11"):
            {
            return "Pre-EQ~/number_obj-11";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("Pre-EQ~/number_obj-3"):
            {
            return "Pre-EQ~/number_obj-3";
            }
        case TAG("Pre-EQ~/number_obj-29"):
            {
            return "Pre-EQ~/number_obj-29";
            }
        case TAG("Pre-EQ~/number_obj-33"):
            {
            return "Pre-EQ~/number_obj-33";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        RNBO_ASSERT(!this->_isInitialized);
        this->assign_defaults();
        this->applyState();
        this->_isInitialized = true;
    }
    
    protected:
    
    void updateTime(MillisecondTime time, INTERNALENGINE*, bool inProcess = false) {
    	if (time == TimeNow) time = getTopLevelPatcher()->getPatcherTime();
    	getTopLevelPatcher()->processInternalEvents(time);
    	updateTime(time, (EXTERNALENGINE*)nullptr);
    }
    
    RNBOSubpatcher_04* operator->() {
        return this;
    }
    const RNBOSubpatcher_04* operator->() const {
        return this;
    }
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1841075377, false);
        getEngine()->flushClockEvents(this, -281953904, false);
    }
    
    UInt64 currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    inline number safediv(number num, number denom) {
        return (denom == 0.0 ? 0.0 : num / denom);
    }
    
    number safepow(number base, number exponent) {
        return fixnan(rnbo_pow(base, exponent));
    }
    
    number scale(
        number x,
        number lowin,
        number hiin,
        number lowout,
        number highout,
        number pow
    ) {
        auto inscale = this->safediv(1., hiin - lowin);
        number outdiff = highout - lowout;
        number value = (x - lowin) * inscale;
    
        if (pow != 1) {
            if (value > 0)
                value = this->safepow(value, pow);
            else
                value = -this->safepow(-value, pow);
        }
    
        value = value * outdiff + lowout;
        return value;
    }
    
    MillisecondTime getPatcherTime() const {
        return this->_currentTime;
    }
    
    void numberobj_01_valin_set(number v) {
        this->numberobj_01_value_set(v);
    }
    
    void numberobj_01_format_set(number v) {
        this->numberobj_01_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numberobj_02_valin_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void numberobj_02_format_set(number v) {
        this->numberobj_02_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_01_out1_bang_bang() {
        this->numberobj_02_value_bang();
    }
    
    template<typename LISTTYPE> void eventinlet_01_out1_list_set(const LISTTYPE& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_02_value_set(converted);
        }
    }
    
    void numberobj_03_valin_set(number v) {
        this->numberobj_03_value_set(v);
    }
    
    void numberobj_03_format_set(number v) {
        this->numberobj_03_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numberobj_04_valin_set(number v) {
        this->numberobj_04_value_set(v);
    }
    
    void numberobj_04_format_set(number v) {
        this->numberobj_04_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_02_out1_bang_bang() {
        this->numberobj_04_value_bang();
    }
    
    template<typename LISTTYPE> void eventinlet_02_out1_list_set(const LISTTYPE& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_04_value_set(converted);
        }
    }
    
    void linetilde_01_target_bang() {}
    
    void linetilde_02_target_bang() {}
    
    void deallocateSignals() {
        Index i;
    
        for (i = 0; i < 3; i++) {
            this->signals[i] = freeSignal(this->signals[i]);
        }
    
        this->zeroBuffer = freeSignal(this->zeroBuffer);
        this->dummyBuffer = freeSignal(this->dummyBuffer);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    void setProbingTarget(MessageTag ) {}
    
    void initializeObjects() {
        this->numberobj_01_init();
        this->numberobj_02_init();
        this->numberobj_03_init();
        this->numberobj_04_init();
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    void onSampleRateChanged(double ) {}
    
    void extractState(PatcherStateInterface& ) {}
    
    void applyState() {}
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time, (ENGINE*)nullptr);
    
        switch (index) {
        case -1841075377:
            {
            this->linetilde_01_target_bang();
            break;
            }
        case -281953904:
            {
            this->linetilde_02_target_bang();
            break;
            }
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time, (ENGINE*)nullptr);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void allocateDataRefs() {}
    
    void linetilde_01_time_set(number v) {
        this->linetilde_01_time = v;
    }
    
    template<typename LISTTYPE> void linetilde_01_segments_set(const LISTTYPE& v) {
        this->linetilde_01_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_01_time == 0) {
                this->linetilde_01_activeRamps->length = 0;
                this->linetilde_01_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_01_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_01_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_01_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_01_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_01_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_01_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_01_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_01_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_01_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_01_activeRamps->push(lastRampValue);
                    this->linetilde_01_activeRamps->push(0);
                    this->linetilde_01_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_01_keepramp)) {
                            this->linetilde_01_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_01_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_01_activeRamps->push(destinationValue);
                    this->linetilde_01_activeRamps->push(inc);
                    this->linetilde_01_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void numberobj_01_output_set(number v) {
        {
            listbase<number, 1> converted = {v};
            this->linetilde_01_segments_set(converted);
        }
    }
    
    void numberobj_01_value_set(number v) {
        this->numberobj_01_value_setter(v);
        v = this->numberobj_01_value;
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_01_output_set(localvalue);
    }
    
    template<typename LISTTYPE> void scale_01_out_set(const LISTTYPE& v) {
        this->scale_01_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_01_value_set(converted);
        }
    }
    
    template<typename LISTTYPE> void scale_01_input_set(const LISTTYPE& v) {
        this->scale_01_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_01_inlow,
                this->scale_01_inhigh,
                this->scale_01_outlow,
                this->scale_01_outhigh,
                this->scale_01_power
            ));
        }
    
        this->scale_01_out_set(tmp);
    }
    
    void numberobj_02_output_set(number v) {
        {
            listbase<number, 1> converted = {v};
            this->scale_01_input_set(converted);
        }
    }
    
    void numberobj_02_value_set(number v) {
        this->numberobj_02_value_setter(v);
        v = this->numberobj_02_value;
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_02_output_set(localvalue);
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void linetilde_02_time_set(number v) {
        this->linetilde_02_time = v;
    }
    
    template<typename LISTTYPE> void linetilde_02_segments_set(const LISTTYPE& v) {
        this->linetilde_02_segments = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->linetilde_02_time == 0) {
                this->linetilde_02_activeRamps->length = 0;
                this->linetilde_02_currentValue = v[0];
            } else {
                auto currentTime = this->currentsampletime();
                number lastRampValue = this->linetilde_02_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
    
                for (Index i = 0; i < this->linetilde_02_activeRamps->length; i += 3) {
                    rampEnd = this->linetilde_02_activeRamps[(Index)(i + 2)];
    
                    if (rampEnd > currentTime) {
                        this->linetilde_02_activeRamps[(Index)(i + 2)] = currentTime;
                        number diff = rampEnd - currentTime;
                        number valueDiff = diff * this->linetilde_02_activeRamps[(Index)(i + 1)];
                        lastRampValue = this->linetilde_02_activeRamps[(Index)i] - valueDiff;
                        this->linetilde_02_activeRamps[(Index)i] = lastRampValue;
                        this->linetilde_02_activeRamps->length = i + 3;
                        rampEnd = currentTime;
                    } else {
                        lastRampValue = this->linetilde_02_activeRamps[(Index)i];
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->linetilde_02_activeRamps->push(lastRampValue);
                    this->linetilde_02_activeRamps->push(0);
                    this->linetilde_02_activeRamps->push(currentTime);
                }
    
                number lastRampEnd = currentTime;
    
                for (Index i = 0; i < v->length; i += 2) {
                    number destinationValue = v[(Index)i];
                    number inc = 0;
                    number rampTimeInSamples;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
    
                        if ((bool)(this->linetilde_02_keepramp)) {
                            this->linetilde_02_time_set(v[(Index)(i + 1)]);
                        }
                    } else {
                        rampTimeInSamples = this->mstosamps(this->linetilde_02_time);
                    }
    
                    if (rampTimeInSamples <= 0) {
                        rampTimeInSamples = 1;
                    }
    
                    inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                    lastRampEnd += rampTimeInSamples;
                    this->linetilde_02_activeRamps->push(destinationValue);
                    this->linetilde_02_activeRamps->push(inc);
                    this->linetilde_02_activeRamps->push(lastRampEnd);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void numberobj_03_output_set(number v) {
        {
            listbase<number, 1> converted = {v};
            this->linetilde_02_segments_set(converted);
        }
    }
    
    void numberobj_03_value_set(number v) {
        this->numberobj_03_value_setter(v);
        v = this->numberobj_03_value;
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_03_output_set(localvalue);
    }
    
    template<typename LISTTYPE> void scale_02_out_set(const LISTTYPE& v) {
        this->scale_02_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_03_value_set(converted);
        }
    }
    
    template<typename LISTTYPE> void scale_02_input_set(const LISTTYPE& v) {
        this->scale_02_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_02_inlow,
                this->scale_02_inhigh,
                this->scale_02_outlow,
                this->scale_02_outhigh,
                this->scale_02_power
            ));
        }
    
        this->scale_02_out_set(tmp);
    }
    
    void numberobj_04_output_set(number v) {
        {
            listbase<number, 1> converted = {v};
            this->scale_02_input_set(converted);
        }
    }
    
    void numberobj_04_value_set(number v) {
        this->numberobj_04_value_setter(v);
        v = this->numberobj_04_value;
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_04_output_set(localvalue);
    }
    
    void eventinlet_02_out1_number_set(number v) {
        this->numberobj_04_value_set(v);
    }
    
    void numberobj_02_value_bang() {
        number v = this->numberobj_02_value;
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_02_output_set(localvalue);
    }
    
    void numberobj_04_value_bang() {
        number v = this->numberobj_04_value;
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_04_output_set(localvalue);
    }
    
    void linetilde_01_perform(SampleValue * out, Index n) {
        auto __linetilde_01_time = this->linetilde_01_time;
        auto __linetilde_01_keepramp = this->linetilde_01_keepramp;
        auto __linetilde_01_currentValue = this->linetilde_01_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_01_activeRamps->length)) {
            while ((bool)(this->linetilde_01_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_01_activeRamps[0];
                number inc = this->linetilde_01_activeRamps[1];
                number rampTimeInSamples = this->linetilde_01_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_01_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_01_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_01_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -1841075377,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_01_keepramp))) {
                            __linetilde_01_time = 0;
                        }
                    }
                }
    
                __linetilde_01_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_01_currentValue;
            i++;
        }
    
        this->linetilde_01_currentValue = __linetilde_01_currentValue;
        this->linetilde_01_time = __linetilde_01_time;
    }
    
    void onepole_tilde_02_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_02_lastY = this->onepole_tilde_02_lastY;
        auto __onepole_tilde_02_b1 = this->onepole_tilde_02_b1;
        auto __onepole_tilde_02_a0 = this->onepole_tilde_02_a0;
        auto __onepole_tilde_02_needsUpdate = this->onepole_tilde_02_needsUpdate;
        auto __onepole_tilde_02_freq = this->onepole_tilde_02_freq;
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            if (__onepole_tilde_02_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_02_needsUpdate)) {
                __onepole_tilde_02_freq = freqInHz[(Index)i];
                __onepole_tilde_02_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_02_a0 = (__onepole_tilde_02_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_02_a0 < 0.00001 ? 0.00001 : __onepole_tilde_02_a0));
                __onepole_tilde_02_b1 = 1 - __onepole_tilde_02_a0;
                __onepole_tilde_02_needsUpdate = false;
            }
    
            __onepole_tilde_02_lastY = __onepole_tilde_02_a0 * x[(Index)i] + __onepole_tilde_02_b1 * __onepole_tilde_02_lastY;
            out1[(Index)i] = __onepole_tilde_02_lastY;
        }
    
        this->onepole_tilde_02_freq = __onepole_tilde_02_freq;
        this->onepole_tilde_02_needsUpdate = __onepole_tilde_02_needsUpdate;
        this->onepole_tilde_02_a0 = __onepole_tilde_02_a0;
        this->onepole_tilde_02_b1 = __onepole_tilde_02_b1;
        this->onepole_tilde_02_lastY = __onepole_tilde_02_lastY;
    }
    
    void dspexpr_01_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void onepole_tilde_04_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_04_lastY = this->onepole_tilde_04_lastY;
        auto __onepole_tilde_04_b1 = this->onepole_tilde_04_b1;
        auto __onepole_tilde_04_a0 = this->onepole_tilde_04_a0;
        auto __onepole_tilde_04_needsUpdate = this->onepole_tilde_04_needsUpdate;
        auto __onepole_tilde_04_freq = this->onepole_tilde_04_freq;
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            if (__onepole_tilde_04_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_04_needsUpdate)) {
                __onepole_tilde_04_freq = freqInHz[(Index)i];
                __onepole_tilde_04_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_04_a0 = (__onepole_tilde_04_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_04_a0 < 0.00001 ? 0.00001 : __onepole_tilde_04_a0));
                __onepole_tilde_04_b1 = 1 - __onepole_tilde_04_a0;
                __onepole_tilde_04_needsUpdate = false;
            }
    
            __onepole_tilde_04_lastY = __onepole_tilde_04_a0 * x[(Index)i] + __onepole_tilde_04_b1 * __onepole_tilde_04_lastY;
            out1[(Index)i] = __onepole_tilde_04_lastY;
        }
    
        this->onepole_tilde_04_freq = __onepole_tilde_04_freq;
        this->onepole_tilde_04_needsUpdate = __onepole_tilde_04_needsUpdate;
        this->onepole_tilde_04_a0 = __onepole_tilde_04_a0;
        this->onepole_tilde_04_b1 = __onepole_tilde_04_b1;
        this->onepole_tilde_04_lastY = __onepole_tilde_04_lastY;
    }
    
    void dspexpr_02_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void linetilde_02_perform(SampleValue * out, Index n) {
        auto __linetilde_02_time = this->linetilde_02_time;
        auto __linetilde_02_keepramp = this->linetilde_02_keepramp;
        auto __linetilde_02_currentValue = this->linetilde_02_currentValue;
        Index i = 0;
    
        if ((bool)(this->linetilde_02_activeRamps->length)) {
            while ((bool)(this->linetilde_02_activeRamps->length) && i < n) {
                number destinationValue = this->linetilde_02_activeRamps[0];
                number inc = this->linetilde_02_activeRamps[1];
                number rampTimeInSamples = this->linetilde_02_activeRamps[2] - this->audioProcessSampleCount - i;
                number val = __linetilde_02_currentValue;
    
                while (rampTimeInSamples > 0 && i < n) {
                    out[(Index)i] = val;
                    val += inc;
                    i++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    val = destinationValue;
                    this->linetilde_02_activeRamps->splice(0, 3);
    
                    if ((bool)(!(bool)(this->linetilde_02_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            -281953904,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        if ((bool)(!(bool)(__linetilde_02_keepramp))) {
                            __linetilde_02_time = 0;
                        }
                    }
                }
    
                __linetilde_02_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __linetilde_02_currentValue;
            i++;
        }
    
        this->linetilde_02_currentValue = __linetilde_02_currentValue;
        this->linetilde_02_time = __linetilde_02_time;
    }
    
    void onepole_tilde_03_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_03_lastY = this->onepole_tilde_03_lastY;
        auto __onepole_tilde_03_b1 = this->onepole_tilde_03_b1;
        auto __onepole_tilde_03_a0 = this->onepole_tilde_03_a0;
        auto __onepole_tilde_03_needsUpdate = this->onepole_tilde_03_needsUpdate;
        auto __onepole_tilde_03_freq = this->onepole_tilde_03_freq;
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            if (__onepole_tilde_03_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_03_needsUpdate)) {
                __onepole_tilde_03_freq = freqInHz[(Index)i];
                __onepole_tilde_03_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_03_a0 = (__onepole_tilde_03_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_03_a0 < 0.00001 ? 0.00001 : __onepole_tilde_03_a0));
                __onepole_tilde_03_b1 = 1 - __onepole_tilde_03_a0;
                __onepole_tilde_03_needsUpdate = false;
            }
    
            __onepole_tilde_03_lastY = __onepole_tilde_03_a0 * x[(Index)i] + __onepole_tilde_03_b1 * __onepole_tilde_03_lastY;
            out1[(Index)i] = __onepole_tilde_03_lastY;
        }
    
        this->onepole_tilde_03_freq = __onepole_tilde_03_freq;
        this->onepole_tilde_03_needsUpdate = __onepole_tilde_03_needsUpdate;
        this->onepole_tilde_03_a0 = __onepole_tilde_03_a0;
        this->onepole_tilde_03_b1 = __onepole_tilde_03_b1;
        this->onepole_tilde_03_lastY = __onepole_tilde_03_lastY;
    }
    
    void onepole_tilde_01_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_01_lastY = this->onepole_tilde_01_lastY;
        auto __onepole_tilde_01_b1 = this->onepole_tilde_01_b1;
        auto __onepole_tilde_01_a0 = this->onepole_tilde_01_a0;
        auto __onepole_tilde_01_needsUpdate = this->onepole_tilde_01_needsUpdate;
        auto __onepole_tilde_01_freq = this->onepole_tilde_01_freq;
        Index i;
    
        for (i = 0; i < (Index)n; i++) {
            if (__onepole_tilde_01_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_01_needsUpdate)) {
                __onepole_tilde_01_freq = freqInHz[(Index)i];
                __onepole_tilde_01_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_01_a0 = (__onepole_tilde_01_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_01_a0 < 0.00001 ? 0.00001 : __onepole_tilde_01_a0));
                __onepole_tilde_01_b1 = 1 - __onepole_tilde_01_a0;
                __onepole_tilde_01_needsUpdate = false;
            }
    
            __onepole_tilde_01_lastY = __onepole_tilde_01_a0 * x[(Index)i] + __onepole_tilde_01_b1 * __onepole_tilde_01_lastY;
            out1[(Index)i] = __onepole_tilde_01_lastY;
        }
    
        this->onepole_tilde_01_freq = __onepole_tilde_01_freq;
        this->onepole_tilde_01_needsUpdate = __onepole_tilde_01_needsUpdate;
        this->onepole_tilde_01_a0 = __onepole_tilde_01_a0;
        this->onepole_tilde_01_b1 = __onepole_tilde_01_b1;
        this->onepole_tilde_01_lastY = __onepole_tilde_01_lastY;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_01_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_01_value = localvalue;
    }
    
    void numberobj_02_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_02_value = localvalue;
    }
    
    void numberobj_03_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_03_value = localvalue;
    }
    
    void numberobj_04_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_04_value = localvalue;
    }
    
    void onepole_tilde_01_reset() {
        this->onepole_tilde_01_lastY = 0;
        this->onepole_tilde_01_a0 = 0;
        this->onepole_tilde_01_b1 = 0;
    }
    
    void onepole_tilde_01_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_01_needsUpdate = true;
        this->onepole_tilde_01_reset();
        this->onepole_tilde_01_setupDone = true;
    }
    
    void onepole_tilde_02_reset() {
        this->onepole_tilde_02_lastY = 0;
        this->onepole_tilde_02_a0 = 0;
        this->onepole_tilde_02_b1 = 0;
    }
    
    void onepole_tilde_02_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_02_needsUpdate = true;
        this->onepole_tilde_02_reset();
        this->onepole_tilde_02_setupDone = true;
    }
    
    void onepole_tilde_03_reset() {
        this->onepole_tilde_03_lastY = 0;
        this->onepole_tilde_03_a0 = 0;
        this->onepole_tilde_03_b1 = 0;
    }
    
    void onepole_tilde_03_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_03_needsUpdate = true;
        this->onepole_tilde_03_reset();
        this->onepole_tilde_03_setupDone = true;
    }
    
    void onepole_tilde_04_reset() {
        this->onepole_tilde_04_lastY = 0;
        this->onepole_tilde_04_a0 = 0;
        this->onepole_tilde_04_b1 = 0;
    }
    
    void onepole_tilde_04_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_04_needsUpdate = true;
        this->onepole_tilde_04_reset();
        this->onepole_tilde_04_setupDone = true;
    }
    
    void numberobj_01_init() {
        this->numberobj_01_currentFormat = 6;
    }
    
    void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_01_value;
    }
    
    void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_01_value_set(preset["value"]);
    }
    
    void numberobj_02_init() {
        this->numberobj_02_currentFormat = 6;
    }
    
    void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_02_value;
    }
    
    void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_02_value_set(preset["value"]);
    }
    
    void numberobj_03_init() {
        this->numberobj_03_currentFormat = 6;
    }
    
    void numberobj_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_03_value;
    }
    
    void numberobj_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_03_value_set(preset["value"]);
    }
    
    void numberobj_04_init() {
        this->numberobj_04_currentFormat = 6;
    }
    
    void numberobj_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_04_value;
    }
    
    void numberobj_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_04_value_set(preset["value"]);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void updateTime(MillisecondTime time, EXTERNALENGINE* engine, bool inProcess = false) {
        RNBO_UNUSED(inProcess);
        RNBO_UNUSED(engine);
        this->_currentTime = time;
        auto offset = rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr));
    
        if (offset >= (SampleIndex)(this->vs))
            offset = (SampleIndex)(this->vs) - 1;
    
        if (offset < 0)
            offset = 0;
    
        this->sampleOffsetIntoNextAudioBuffer = (Index)(offset);
    }
    
    void assign_defaults()
    {
        onepole_tilde_01_x = 0;
        onepole_tilde_01_freqInHz = 20000;
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 0;
        onepole_tilde_02_x = 0;
        onepole_tilde_02_freqInHz = 20;
        onepole_tilde_03_x = 0;
        onepole_tilde_03_freqInHz = 20000;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        onepole_tilde_04_x = 0;
        onepole_tilde_04_freqInHz = 20;
        linetilde_01_time = 10;
        linetilde_01_keepramp = false;
        numberobj_01_value = 0;
        numberobj_01_value_setter(numberobj_01_value);
        scale_01_inlow = 0;
        scale_01_inhigh = 100;
        scale_01_outlow = 20;
        scale_01_outhigh = 20000;
        scale_01_power = 4.35;
        numberobj_02_value = 0;
        numberobj_02_value_setter(numberobj_02_value);
        linetilde_02_time = 10;
        linetilde_02_keepramp = false;
        numberobj_03_value = 0;
        numberobj_03_value_setter(numberobj_03_value);
        scale_02_inlow = 0;
        scale_02_inhigh = 100;
        scale_02_outlow = 20000;
        scale_02_outhigh = 20;
        scale_02_power = 0.0725;
        numberobj_04_value = 0;
        numberobj_04_value_setter(numberobj_04_value);
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.000022675736961451248;
        onepole_tilde_01_freq = 0;
        onepole_tilde_01_needsUpdate = false;
        onepole_tilde_01_lastY = 0;
        onepole_tilde_01_a0 = 0;
        onepole_tilde_01_b1 = 0;
        onepole_tilde_01_setupDone = false;
        onepole_tilde_02_freq = 0;
        onepole_tilde_02_needsUpdate = false;
        onepole_tilde_02_lastY = 0;
        onepole_tilde_02_a0 = 0;
        onepole_tilde_02_b1 = 0;
        onepole_tilde_02_setupDone = false;
        onepole_tilde_03_freq = 0;
        onepole_tilde_03_needsUpdate = false;
        onepole_tilde_03_lastY = 0;
        onepole_tilde_03_a0 = 0;
        onepole_tilde_03_b1 = 0;
        onepole_tilde_03_setupDone = false;
        onepole_tilde_04_freq = 0;
        onepole_tilde_04_needsUpdate = false;
        onepole_tilde_04_lastY = 0;
        onepole_tilde_04_a0 = 0;
        onepole_tilde_04_b1 = 0;
        onepole_tilde_04_setupDone = false;
        linetilde_01_currentValue = 20;
        numberobj_01_currentFormat = 6;
        numberobj_01_lastValue = 0;
        numberobj_02_currentFormat = 6;
        numberobj_02_lastValue = 0;
        linetilde_02_currentValue = 20;
        numberobj_03_currentFormat = 6;
        numberobj_03_lastValue = 0;
        numberobj_04_currentFormat = 6;
        numberobj_04_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number onepole_tilde_01_x;
        number onepole_tilde_01_freqInHz;
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number onepole_tilde_02_x;
        number onepole_tilde_02_freqInHz;
        number onepole_tilde_03_x;
        number onepole_tilde_03_freqInHz;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number onepole_tilde_04_x;
        number onepole_tilde_04_freqInHz;
        list linetilde_01_segments;
        number linetilde_01_time;
        number linetilde_01_keepramp;
        number numberobj_01_value;
        list scale_01_input;
        number scale_01_inlow;
        number scale_01_inhigh;
        number scale_01_outlow;
        number scale_01_outhigh;
        number scale_01_power;
        list scale_01_out;
        number numberobj_02_value;
        list linetilde_02_segments;
        number linetilde_02_time;
        number linetilde_02_keepramp;
        number numberobj_03_value;
        list scale_02_input;
        number scale_02_inlow;
        number scale_02_inhigh;
        number scale_02_outlow;
        number scale_02_outhigh;
        number scale_02_power;
        list scale_02_out;
        number numberobj_04_value;
        MillisecondTime _currentTime;
        UInt64 audioProcessSampleCount;
        Index sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[3];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number onepole_tilde_01_freq;
        bool onepole_tilde_01_needsUpdate;
        number onepole_tilde_01_lastY;
        number onepole_tilde_01_a0;
        number onepole_tilde_01_b1;
        bool onepole_tilde_01_setupDone;
        number onepole_tilde_02_freq;
        bool onepole_tilde_02_needsUpdate;
        number onepole_tilde_02_lastY;
        number onepole_tilde_02_a0;
        number onepole_tilde_02_b1;
        bool onepole_tilde_02_setupDone;
        number onepole_tilde_03_freq;
        bool onepole_tilde_03_needsUpdate;
        number onepole_tilde_03_lastY;
        number onepole_tilde_03_a0;
        number onepole_tilde_03_b1;
        bool onepole_tilde_03_setupDone;
        number onepole_tilde_04_freq;
        bool onepole_tilde_04_needsUpdate;
        number onepole_tilde_04_lastY;
        number onepole_tilde_04_a0;
        number onepole_tilde_04_b1;
        bool onepole_tilde_04_setupDone;
        list linetilde_01_activeRamps;
        number linetilde_01_currentValue;
        Int numberobj_01_currentFormat;
        number numberobj_01_lastValue;
        Int numberobj_02_currentFormat;
        number numberobj_02_lastValue;
        list linetilde_02_activeRamps;
        number linetilde_02_currentValue;
        Int numberobj_03_currentFormat;
        number numberobj_03_lastValue;
        Int numberobj_04_currentFormat;
        number numberobj_04_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
        bool _isInitialized = false;
};

		
void advanceTime(EXTERNALENGINE*) {}
void advanceTime(INTERNALENGINE*) {
	_internalEngine.advanceTime(sampstoms(this->vs));
}

void processInternalEvents(MillisecondTime time) {
	_internalEngine.processEventsUntil(time);
}

void updateTime(MillisecondTime time, INTERNALENGINE*, bool inProcess = false) {
	if (time == TimeNow) time = getPatcherTime();
	processInternalEvents(inProcess ? time + sampsToMs(this->vs) : time);
	updateTime(time, (EXTERNALENGINE*)nullptr);
}

rnbomatic* operator->() {
    return this;
}
const rnbomatic* operator->() const {
    return this;
}
rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template<typename LISTTYPE = list> void listquicksort(LISTTYPE& arr, LISTTYPE& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template<typename LISTTYPE = list> Int listpartition(LISTTYPE& arr, LISTTYPE& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template<typename LISTTYPE = list> void listswapelements(LISTTYPE& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

inline number safediv(number num, number denom) {
    return (denom == 0.0 ? 0.0 : num / denom);
}

number safepow(number base, number exponent) {
    return fixnan(rnbo_pow(base, exponent));
}

number scale(
    number x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow
) {
    auto inscale = this->safediv(1., hiin - lowin);
    number outdiff = highout - lowout;
    number value = (x - lowin) * inscale;

    if (pow != 1) {
        if (value > 0)
            value = this->safepow(value, pow);
        else
            value = -this->safepow(-value, pow);
    }

    value = value * outdiff + lowout;
    return value;
}

number fold(number x, number low, number high) {
    number lo;
    number hi;

    if (low == high) {
        return low;
    }

    if (low > high) {
        hi = low;
        lo = high;
    } else {
        lo = low;
        hi = high;
    }

    number range = hi - lo;
    Int numWraps = 0;

    if (x >= hi) {
        x -= range;

        if (x >= hi) {
            numWraps = (x - lo) / range;
            x -= range * numWraps;
        }

        numWraps++;
    } else if (x < lo) {
        x += range;

        if (x < lo) {
            numWraps = (x - lo) / range - 1;
            x -= range * numWraps;
        }

        numWraps--;
    }

    if ((BinOpInt)((BinOpInt)numWraps & (BinOpInt)1))
        x = hi + lo - x;

    return x;
}

number fromnormalized(Index index, number normalizedValue) {
    return this->convertFromNormalizedParameterValue(index, normalizedValue);
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    this->p_01_in3_number_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->p_01_in4_number_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->gen_01_foldMode_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->gen_01_feedCoe_down_set(v);
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    this->gen_01_feedCoe_up_set(v);
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->gen_01_Mix_set(v);
}

void param_07_value_set(number v) {
    v = this->param_07_value_constrain(v);
    this->param_07_value = v;
    this->sendParameter(6, false);

    if (this->param_07_value != this->param_07_lastValue) {
        this->getEngine()->presetTouched();
        this->param_07_lastValue = this->param_07_value;
    }

    this->gen_01_clipType_set(v);
}

void param_08_value_set(number v) {
    v = this->param_08_value_constrain(v);
    this->param_08_value = v;
    this->sendParameter(7, false);

    if (this->param_08_value != this->param_08_lastValue) {
        this->getEngine()->presetTouched();
        this->param_08_lastValue = this->param_08_value;
    }

    this->gen_01_clipNeg_set(v);
}

void param_09_value_set(number v) {
    v = this->param_09_value_constrain(v);
    this->param_09_value = v;
    this->sendParameter(8, false);

    if (this->param_09_value != this->param_09_lastValue) {
        this->getEngine()->presetTouched();
        this->param_09_lastValue = this->param_09_value;
    }

    this->gen_01_clipPos_set(v);
}

MillisecondTime getPatcherTime() const {
    return this->_currentTime;
}

void deallocateSignals() {
    Index i;

    for (i = 0; i < 2; i++) {
        this->signals[i] = freeSignal(this->signals[i]);
    }

    this->globaltransport_tempo = freeSignal(this->globaltransport_tempo);
    this->globaltransport_state = freeSignal(this->globaltransport_state);
    this->zeroBuffer = freeSignal(this->zeroBuffer);
    this->dummyBuffer = freeSignal(this->dummyBuffer);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

void setProbingTarget(MessageTag ) {}

void fillDataRef(DataRefIndex , DataRef& ) {}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
}

void initializeObjects() {
    this->gen_01_softClipValL_init();
    this->gen_01_softClipValR_init();
    this->gen_01_hardClipValL_init();
    this->gen_01_hardClipValR_init();
    this->gen_01_superhardClipValL_init();
    this->gen_01_superhardClipValR_init();
    this->gen_01_change_4_init();
    this->gen_01_change_6_init();
    this->p_01->initializeObjects();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

void onSampleRateChanged(double ) {}

void extractState(PatcherStateInterface& ) {}

void applyState() {

    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
}

ParameterIndex getParameterOffset(BaseInterface& subpatcher) const {
    if (addressOf(subpatcher) == addressOf(this->p_01))
        return 9;

    return 0;
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time, (ENGINE*)nullptr);
    this->processOutletAtCurrentTime(sender, index, value);
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime(), (ENGINE*)nullptr);
    this->p_01->startup();

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    {
        this->scheduleParamInit(4, 0);
    }

    {
        this->scheduleParamInit(5, 0);
    }

    {
        this->scheduleParamInit(6, 0);
    }

    {
        this->scheduleParamInit(7, 0);
    }

    {
        this->scheduleParamInit(8, 0);
    }

    this->processParamInitEvents();
}

number param_01_value_constrain(number v) const {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in3_number_set(number v) {
    this->p_01->updateTime(this->_currentTime, (ENGINE*)nullptr);
    this->p_01->eventinlet_01_out1_number_set(v);
}

number param_02_value_constrain(number v) const {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in4_number_set(number v) {
    this->p_01->updateTime(this->_currentTime, (ENGINE*)nullptr);
    this->p_01->eventinlet_02_out1_number_set(v);
}

number param_03_value_constrain(number v) const {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_foldMode_set(number v) {
    this->gen_01_foldMode = v;
}

number param_04_value_constrain(number v) const {
    v = (v > 0.99 ? 0.99 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_feedCoe_down_set(number v) {
    this->gen_01_feedCoe_down = v;
}

number param_05_value_constrain(number v) const {
    v = (v > 0.99 ? 0.99 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_feedCoe_up_set(number v) {
    this->gen_01_feedCoe_up = v;
}

number param_06_value_constrain(number v) const {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_Mix_set(number v) {
    this->gen_01_Mix = v;
}

number param_07_value_constrain(number v) const {
    v = (v > 2 ? 2 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipType_set(number v) {
    this->gen_01_clipType = v;
}

number param_08_value_constrain(number v) const {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipNeg_set(number v) {
    this->gen_01_clipNeg = v;
}

number param_09_value_constrain(number v) const {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipPos_set(number v) {
    this->gen_01_clipPos = v;
}

void ctlin_01_outchannel_set(number ) {}

void ctlin_01_outcontroller_set(number ) {}

void fromnormalized_01_output_set(number v) {
    this->param_01_value_set(v);
}

void fromnormalized_01_input_set(number v) {
    this->fromnormalized_01_output_set(this->fromnormalized(0, v));
}

void expr_01_out1_set(number v) {
    this->expr_01_out1 = v;
    this->fromnormalized_01_input_set(this->expr_01_out1);
}

void expr_01_in1_set(number in1) {
    this->expr_01_in1 = in1;
    this->expr_01_out1_set(this->expr_01_in1 * this->expr_01_in2);//#map:expr_01:1
}

void ctlin_01_value_set(number v) {
    this->expr_01_in1_set(v);
}

void ctlin_01_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_01_channel || this->ctlin_01_channel == -1) && (data[1] == this->ctlin_01_controller || this->ctlin_01_controller == -1)) {
        this->ctlin_01_outchannel_set(channel);
        this->ctlin_01_outcontroller_set(data[1]);
        this->ctlin_01_value_set(data[2]);
        this->ctlin_01_status = 0;
    }
}

void ctlin_02_outchannel_set(number ) {}

void ctlin_02_outcontroller_set(number ) {}

void fromnormalized_02_output_set(number v) {
    this->param_02_value_set(v);
}

void fromnormalized_02_input_set(number v) {
    this->fromnormalized_02_output_set(this->fromnormalized(1, v));
}

void expr_02_out1_set(number v) {
    this->expr_02_out1 = v;
    this->fromnormalized_02_input_set(this->expr_02_out1);
}

void expr_02_in1_set(number in1) {
    this->expr_02_in1 = in1;
    this->expr_02_out1_set(this->expr_02_in1 * this->expr_02_in2);//#map:expr_02:1
}

void ctlin_02_value_set(number v) {
    this->expr_02_in1_set(v);
}

void ctlin_02_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_02_channel || this->ctlin_02_channel == -1) && (data[1] == this->ctlin_02_controller || this->ctlin_02_controller == -1)) {
        this->ctlin_02_outchannel_set(channel);
        this->ctlin_02_outcontroller_set(data[1]);
        this->ctlin_02_value_set(data[2]);
        this->ctlin_02_status = 0;
    }
}

void ctlin_03_outchannel_set(number ) {}

void ctlin_03_outcontroller_set(number ) {}

void fromnormalized_03_output_set(number v) {
    this->param_03_value_set(v);
}

void fromnormalized_03_input_set(number v) {
    this->fromnormalized_03_output_set(this->fromnormalized(2, v));
}

void expr_03_out1_set(number v) {
    this->expr_03_out1 = v;
    this->fromnormalized_03_input_set(this->expr_03_out1);
}

void expr_03_in1_set(number in1) {
    this->expr_03_in1 = in1;
    this->expr_03_out1_set(this->expr_03_in1 * this->expr_03_in2);//#map:expr_03:1
}

void ctlin_03_value_set(number v) {
    this->expr_03_in1_set(v);
}

void ctlin_03_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_03_channel || this->ctlin_03_channel == -1) && (data[1] == this->ctlin_03_controller || this->ctlin_03_controller == -1)) {
        this->ctlin_03_outchannel_set(channel);
        this->ctlin_03_outcontroller_set(data[1]);
        this->ctlin_03_value_set(data[2]);
        this->ctlin_03_status = 0;
    }
}

void ctlin_04_outchannel_set(number ) {}

void ctlin_04_outcontroller_set(number ) {}

void fromnormalized_04_output_set(number v) {
    this->param_04_value_set(v);
}

void fromnormalized_04_input_set(number v) {
    this->fromnormalized_04_output_set(this->fromnormalized(3, v));
}

void expr_04_out1_set(number v) {
    this->expr_04_out1 = v;
    this->fromnormalized_04_input_set(this->expr_04_out1);
}

void expr_04_in1_set(number in1) {
    this->expr_04_in1 = in1;
    this->expr_04_out1_set(this->expr_04_in1 * this->expr_04_in2);//#map:expr_04:1
}

void ctlin_04_value_set(number v) {
    this->expr_04_in1_set(v);
}

void ctlin_04_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_04_channel || this->ctlin_04_channel == -1) && (data[1] == this->ctlin_04_controller || this->ctlin_04_controller == -1)) {
        this->ctlin_04_outchannel_set(channel);
        this->ctlin_04_outcontroller_set(data[1]);
        this->ctlin_04_value_set(data[2]);
        this->ctlin_04_status = 0;
    }
}

void ctlin_05_outchannel_set(number ) {}

void ctlin_05_outcontroller_set(number ) {}

void fromnormalized_05_output_set(number v) {
    this->param_05_value_set(v);
}

void fromnormalized_05_input_set(number v) {
    this->fromnormalized_05_output_set(this->fromnormalized(4, v));
}

void expr_05_out1_set(number v) {
    this->expr_05_out1 = v;
    this->fromnormalized_05_input_set(this->expr_05_out1);
}

void expr_05_in1_set(number in1) {
    this->expr_05_in1 = in1;
    this->expr_05_out1_set(this->expr_05_in1 * this->expr_05_in2);//#map:expr_05:1
}

void ctlin_05_value_set(number v) {
    this->expr_05_in1_set(v);
}

void ctlin_05_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_05_channel || this->ctlin_05_channel == -1) && (data[1] == this->ctlin_05_controller || this->ctlin_05_controller == -1)) {
        this->ctlin_05_outchannel_set(channel);
        this->ctlin_05_outcontroller_set(data[1]);
        this->ctlin_05_value_set(data[2]);
        this->ctlin_05_status = 0;
    }
}

void ctlin_06_outchannel_set(number ) {}

void ctlin_06_outcontroller_set(number ) {}

void fromnormalized_06_output_set(number v) {
    this->param_06_value_set(v);
}

void fromnormalized_06_input_set(number v) {
    this->fromnormalized_06_output_set(this->fromnormalized(5, v));
}

void expr_06_out1_set(number v) {
    this->expr_06_out1 = v;
    this->fromnormalized_06_input_set(this->expr_06_out1);
}

void expr_06_in1_set(number in1) {
    this->expr_06_in1 = in1;
    this->expr_06_out1_set(this->expr_06_in1 * this->expr_06_in2);//#map:expr_06:1
}

void ctlin_06_value_set(number v) {
    this->expr_06_in1_set(v);
}

void ctlin_06_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_06_channel || this->ctlin_06_channel == -1) && (data[1] == this->ctlin_06_controller || this->ctlin_06_controller == -1)) {
        this->ctlin_06_outchannel_set(channel);
        this->ctlin_06_outcontroller_set(data[1]);
        this->ctlin_06_value_set(data[2]);
        this->ctlin_06_status = 0;
    }
}

void ctlin_07_outchannel_set(number ) {}

void ctlin_07_outcontroller_set(number ) {}

void fromnormalized_07_output_set(number v) {
    this->param_07_value_set(v);
}

void fromnormalized_07_input_set(number v) {
    this->fromnormalized_07_output_set(this->fromnormalized(6, v));
}

void expr_07_out1_set(number v) {
    this->expr_07_out1 = v;
    this->fromnormalized_07_input_set(this->expr_07_out1);
}

void expr_07_in1_set(number in1) {
    this->expr_07_in1 = in1;
    this->expr_07_out1_set(this->expr_07_in1 * this->expr_07_in2);//#map:expr_07:1
}

void ctlin_07_value_set(number v) {
    this->expr_07_in1_set(v);
}

void ctlin_07_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_07_channel || this->ctlin_07_channel == -1) && (data[1] == this->ctlin_07_controller || this->ctlin_07_controller == -1)) {
        this->ctlin_07_outchannel_set(channel);
        this->ctlin_07_outcontroller_set(data[1]);
        this->ctlin_07_value_set(data[2]);
        this->ctlin_07_status = 0;
    }
}

void ctlin_08_outchannel_set(number ) {}

void ctlin_08_outcontroller_set(number ) {}

void fromnormalized_08_output_set(number v) {
    this->param_08_value_set(v);
}

void fromnormalized_08_input_set(number v) {
    this->fromnormalized_08_output_set(this->fromnormalized(7, v));
}

void expr_08_out1_set(number v) {
    this->expr_08_out1 = v;
    this->fromnormalized_08_input_set(this->expr_08_out1);
}

void expr_08_in1_set(number in1) {
    this->expr_08_in1 = in1;
    this->expr_08_out1_set(this->expr_08_in1 * this->expr_08_in2);//#map:expr_08:1
}

void ctlin_08_value_set(number v) {
    this->expr_08_in1_set(v);
}

void ctlin_08_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_08_channel || this->ctlin_08_channel == -1) && (data[1] == this->ctlin_08_controller || this->ctlin_08_controller == -1)) {
        this->ctlin_08_outchannel_set(channel);
        this->ctlin_08_outcontroller_set(data[1]);
        this->ctlin_08_value_set(data[2]);
        this->ctlin_08_status = 0;
    }
}

void ctlin_09_outchannel_set(number ) {}

void ctlin_09_outcontroller_set(number ) {}

void fromnormalized_09_output_set(number v) {
    this->param_09_value_set(v);
}

void fromnormalized_09_input_set(number v) {
    this->fromnormalized_09_output_set(this->fromnormalized(8, v));
}

void expr_09_out1_set(number v) {
    this->expr_09_out1 = v;
    this->fromnormalized_09_input_set(this->expr_09_out1);
}

void expr_09_in1_set(number in1) {
    this->expr_09_in1 = in1;
    this->expr_09_out1_set(this->expr_09_in1 * this->expr_09_in2);//#map:expr_09:1
}

void ctlin_09_value_set(number v) {
    this->expr_09_in1_set(v);
}

void ctlin_09_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_09_channel || this->ctlin_09_channel == -1) && (data[1] == this->ctlin_09_controller || this->ctlin_09_controller == -1)) {
        this->ctlin_09_outchannel_set(channel);
        this->ctlin_09_outcontroller_set(data[1]);
        this->ctlin_09_value_set(data[2]);
        this->ctlin_09_status = 0;
    }
}

void p_01_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Pre-EQ~
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 2, outs, 2, n);
}

void gen_01_perform(
    const Sample * in1,
    const Sample * in2,
    number clipPos,
    number clipNeg,
    number clipType,
    number feedCoe_up,
    number feedCoe_down,
    number foldMode,
    number Mix,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    auto __gen_01_superhardClipValR_value = this->gen_01_superhardClipValR_value;
    auto __gen_01_superhardClipValL_value = this->gen_01_superhardClipValL_value;
    auto __gen_01_hardClipValR_value = this->gen_01_hardClipValR_value;
    auto __gen_01_hardClipValL_value = this->gen_01_hardClipValL_value;
    auto __gen_01_softClipValR_value = this->gen_01_softClipValR_value;
    auto __gen_01_softClipValL_value = this->gen_01_softClipValL_value;
    Index i;

    for (i = 0; i < (Index)n; i++) {
        number mixed1L_0 = 0;
        number mixed2L_1 = 0;
        number mixed1R_2 = 0;
        number mixed2R_3 = 0;
        number current_fbL_5 = (this->gen_01_change_4_next(in1[(Index)i]) > 0 ? feedCoe_up : feedCoe_down);
        number current_fbR_7 = (this->gen_01_change_6_next(in2[(Index)i]) > 0 ? feedCoe_up : feedCoe_down);
        number softClip_8 = 0;

        {
            auto foldMode_12 = foldMode;
            auto val2_11 = clipNeg;
            auto val1_10 = clipPos;
            auto input_9 = in1[(Index)i] + current_fbL_5 * __gen_01_softClipValL_value;
            number value0_13 = 0;
            number value1_14 = 0;
            number value2_15 = 0;
            auto scaledClip1_16 = this->scale(val1_10, 100, 0, 0, 1, 1);
            auto scaledClip2_17 = this->scale(val2_11, 100, 0, 0, 1, 1);
            auto scaledClip3_18 = this->scale(val1_10, 0, 100, 1, 10, 1);
            auto scaledClip4_19 = this->scale(val2_11, 0, 100, 1, 10, 1);
            auto scaledClip5_20 = this->scale(val1_10, 0, 100, 1.9, 1, 0.2);
            auto scaledClip6_21 = this->scale(val2_11, 0, 100, 1.9, 1, 0.2);

            if (foldMode_12 == 0) {
                value0_13 = input_9;
            } else {
                value0_13 = this->fold(input_9, -scaledClip2_17, scaledClip1_16);
            }

            if (input_9 >= 0) {
                value1_14 = scaledClip5_20 * rnbo_atan(value0_13 * scaledClip3_18);
            } else {
                value2_15 = scaledClip6_21 * rnbo_atan(value0_13 * scaledClip4_19);
            }

            softClip_8 = ((value1_14 + value2_15) * 2 / (number)3.141592653589793 > 1 ? 1 : ((value1_14 + value2_15) * 2 / (number)3.141592653589793 < -1 ? -1 : (value1_14 + value2_15) * 2 / (number)3.141592653589793));
        }

        __gen_01_softClipValL_value = softClip_8;
        number softClip_22 = 0;

        {
            auto foldMode_26 = foldMode;
            auto val2_25 = clipNeg;
            auto val1_24 = clipPos;
            auto input_23 = in2[(Index)i] + current_fbR_7 * __gen_01_softClipValR_value;
            number value0_27 = 0;
            number value1_28 = 0;
            number value2_29 = 0;
            auto scaledClip1_30 = this->scale(val1_24, 100, 0, 0, 1, 1);
            auto scaledClip2_31 = this->scale(val2_25, 100, 0, 0, 1, 1);
            auto scaledClip3_32 = this->scale(val1_24, 0, 100, 1, 10, 1);
            auto scaledClip4_33 = this->scale(val2_25, 0, 100, 1, 10, 1);
            auto scaledClip5_34 = this->scale(val1_24, 0, 100, 1.9, 1, 0.2);
            auto scaledClip6_35 = this->scale(val2_25, 0, 100, 1.9, 1, 0.2);

            if (foldMode_26 == 0) {
                value0_27 = input_23;
            } else {
                value0_27 = this->fold(input_23, -scaledClip2_31, scaledClip1_30);
            }

            if (input_23 >= 0) {
                value1_28 = scaledClip5_34 * rnbo_atan(value0_27 * scaledClip3_32);
            } else {
                value2_29 = scaledClip6_35 * rnbo_atan(value0_27 * scaledClip4_33);
            }

            softClip_22 = ((value1_28 + value2_29) * 2 / (number)3.141592653589793 > 1 ? 1 : ((value1_28 + value2_29) * 2 / (number)3.141592653589793 < -1 ? -1 : (value1_28 + value2_29) * 2 / (number)3.141592653589793));
        }

        __gen_01_softClipValR_value = softClip_22;
        number hardClip_36 = 0;

        {
            auto foldMode_40 = foldMode;
            auto val2_39 = clipNeg;
            auto val1_38 = clipPos;
            auto input_37 = in1[(Index)i] + current_fbL_5 * __gen_01_hardClipValL_value;
            number value0_41 = 0;
            number value1_42 = 0;
            number value2_43 = 0;
            auto scaledClip1_44 = this->scale(val1_38, 100, 0, 0, 1, 1);
            auto scaledClip2_45 = this->scale(val2_39, 100, 0, 0, 1, 1);
            auto scaledClip3_46 = this->scale(val1_38, 0, 100, 1, 10, 1);
            auto scaledClip4_47 = this->scale(val2_39, 0, 100, 1, 10, 1);
            auto scaledClip5_48 = this->scale(val1_38, 0, 100, 1.25, 0.9, 0.15);
            auto scaledClip6_49 = this->scale(val2_39, 0, 100, 1.25, 0.9, 0.15);

            if (foldMode_40 == 0) {
                value0_41 = input_37;
            } else {
                value0_41 = this->fold(input_37, -scaledClip2_45, scaledClip1_44);
            }

            if (input_37 >= 0) {
                value1_42 = scaledClip5_48 * rnbo_tanh(value0_41 * scaledClip3_46);
            } else {
                value2_43 = scaledClip6_49 * rnbo_tanh(value0_41 * scaledClip4_47);
            }

            hardClip_36 = (value1_42 + value2_43 > 1 ? 1 : (value1_42 + value2_43 < -1 ? -1 : value1_42 + value2_43));
        }

        __gen_01_hardClipValL_value = hardClip_36;
        number hardClip_50 = 0;

        {
            auto foldMode_54 = foldMode;
            auto val2_53 = clipNeg;
            auto val1_52 = clipPos;
            auto input_51 = in2[(Index)i] + current_fbR_7 * __gen_01_hardClipValR_value;
            number value0_55 = 0;
            number value1_56 = 0;
            number value2_57 = 0;
            auto scaledClip1_58 = this->scale(val1_52, 100, 0, 0, 1, 1);
            auto scaledClip2_59 = this->scale(val2_53, 100, 0, 0, 1, 1);
            auto scaledClip3_60 = this->scale(val1_52, 0, 100, 1, 10, 1);
            auto scaledClip4_61 = this->scale(val2_53, 0, 100, 1, 10, 1);
            auto scaledClip5_62 = this->scale(val1_52, 0, 100, 1.25, 0.9, 0.15);
            auto scaledClip6_63 = this->scale(val2_53, 0, 100, 1.25, 0.9, 0.15);

            if (foldMode_54 == 0) {
                value0_55 = input_51;
            } else {
                value0_55 = this->fold(input_51, -scaledClip2_59, scaledClip1_58);
            }

            if (input_51 >= 0) {
                value1_56 = scaledClip5_62 * rnbo_tanh(value0_55 * scaledClip3_60);
            } else {
                value2_57 = scaledClip6_63 * rnbo_tanh(value0_55 * scaledClip4_61);
            }

            hardClip_50 = (value1_56 + value2_57 > 1 ? 1 : (value1_56 + value2_57 < -1 ? -1 : value1_56 + value2_57));
        }

        __gen_01_hardClipValR_value = hardClip_50;
        number superhardClip_64 = 0;

        {
            auto foldMode_68 = foldMode;
            auto val2_67 = clipNeg;
            auto val1_66 = clipPos;
            auto input_65 = in1[(Index)i] + current_fbL_5 * __gen_01_superhardClipValL_value;
            auto scaledClip1_69 = this->scale(val1_66, 100, 0, 0, 1, 1);
            auto scaledClip2_70 = this->scale(val2_67, 100, 0, 0, 1, 1);
            number div5_71 = fixnan(rnbo_pow(scaledClip1_69 * 10, 2)) / (number)100;
            number div6_72 = fixnan(rnbo_pow(scaledClip2_70 * 10, 2)) / (number)100;
            number value1_73 = 0;

            if (foldMode_68 == 0) {
                value1_73 = (input_65 > div5_71 ? div5_71 : (input_65 < -div6_72 ? -div6_72 : input_65));
            } else {
                value1_73 = this->fold(input_65, -div6_72, div5_71);
            }

            number div10_74 = (-div6_72 + div5_71) / (number)2;
            number div13_75 = (div5_71 + div6_72) / (number)2;
            number rdiv14_76 = (div13_75 == 0. ? 0. : (number)1 / div13_75);
            superhardClip_64 = ((value1_73 - div10_74) * rdiv14_76 * 0.9 > 1 ? 1 : ((value1_73 - div10_74) * rdiv14_76 * 0.9 < -1 ? -1 : (value1_73 - div10_74) * rdiv14_76 * 0.9));
        }

        __gen_01_superhardClipValL_value = superhardClip_64;
        number superhardClip_77 = 0;

        {
            auto foldMode_81 = foldMode;
            auto val2_80 = clipNeg;
            auto val1_79 = clipPos;
            auto input_78 = in2[(Index)i] + current_fbR_7 * __gen_01_superhardClipValR_value;
            auto scaledClip1_82 = this->scale(val1_79, 100, 0, 0, 1, 1);
            auto scaledClip2_83 = this->scale(val2_80, 100, 0, 0, 1, 1);
            number div5_84 = fixnan(rnbo_pow(scaledClip1_82 * 10, 2)) / (number)100;
            number div6_85 = fixnan(rnbo_pow(scaledClip2_83 * 10, 2)) / (number)100;
            number value1_86 = 0;

            if (foldMode_81 == 0) {
                value1_86 = (input_78 > div5_84 ? div5_84 : (input_78 < -div6_85 ? -div6_85 : input_78));
            } else {
                value1_86 = this->fold(input_78, -div6_85, div5_84);
            }

            number div10_87 = (-div6_85 + div5_84) / (number)2;
            number div13_88 = (div5_84 + div6_85) / (number)2;
            number rdiv14_89 = (div13_88 == 0. ? 0. : (number)1 / div13_88);
            superhardClip_77 = ((value1_86 - div10_87) * rdiv14_89 * 0.9 > 1 ? 1 : ((value1_86 - div10_87) * rdiv14_89 * 0.9 < -1 ? -1 : (value1_86 - div10_87) * rdiv14_89 * 0.9));
        }

        __gen_01_superhardClipValR_value = superhardClip_77;

        if (0 <= clipType && clipType <= 1) {
            mixed1L_0 = __gen_01_softClipValL_value + clipType * (__gen_01_hardClipValL_value - __gen_01_softClipValL_value);
            mixed1R_2 = __gen_01_softClipValR_value + clipType * (__gen_01_hardClipValR_value - __gen_01_softClipValR_value);
        } else {
            mixed2L_1 = __gen_01_hardClipValL_value + (clipType - 1) * (__gen_01_superhardClipValL_value - __gen_01_hardClipValL_value);
            mixed2R_3 = __gen_01_hardClipValR_value + (clipType - 1) * (__gen_01_superhardClipValR_value - __gen_01_hardClipValR_value);
        }

        number expr_1_90 = in1[(Index)i] + Mix / (number)100 * (mixed1L_0 + mixed2L_1 - in1[(Index)i]);
        number expr_2_91 = in2[(Index)i] + Mix / (number)100 * (mixed1R_2 + mixed2R_3 - in2[(Index)i]);
        out1[(Index)i] = expr_1_90;
        out2[(Index)i] = expr_2_91;
    }

    this->gen_01_softClipValL_value = __gen_01_softClipValL_value;
    this->gen_01_softClipValR_value = __gen_01_softClipValR_value;
    this->gen_01_hardClipValL_value = __gen_01_hardClipValL_value;
    this->gen_01_hardClipValR_value = __gen_01_hardClipValR_value;
    this->gen_01_superhardClipValL_value = __gen_01_superhardClipValL_value;
    this->gen_01_superhardClipValR_value = __gen_01_superhardClipValR_value;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

number gen_01_softClipValL_getvalue() {
    return this->gen_01_softClipValL_value;
}

void gen_01_softClipValL_setvalue(number val) {
    this->gen_01_softClipValL_value = val;
}

void gen_01_softClipValL_reset() {
    this->gen_01_softClipValL_value = 0;
}

void gen_01_softClipValL_init() {
    this->gen_01_softClipValL_value = 0;
}

number gen_01_softClipValR_getvalue() {
    return this->gen_01_softClipValR_value;
}

void gen_01_softClipValR_setvalue(number val) {
    this->gen_01_softClipValR_value = val;
}

void gen_01_softClipValR_reset() {
    this->gen_01_softClipValR_value = 0;
}

void gen_01_softClipValR_init() {
    this->gen_01_softClipValR_value = 0;
}

number gen_01_hardClipValL_getvalue() {
    return this->gen_01_hardClipValL_value;
}

void gen_01_hardClipValL_setvalue(number val) {
    this->gen_01_hardClipValL_value = val;
}

void gen_01_hardClipValL_reset() {
    this->gen_01_hardClipValL_value = 0;
}

void gen_01_hardClipValL_init() {
    this->gen_01_hardClipValL_value = 0;
}

number gen_01_hardClipValR_getvalue() {
    return this->gen_01_hardClipValR_value;
}

void gen_01_hardClipValR_setvalue(number val) {
    this->gen_01_hardClipValR_value = val;
}

void gen_01_hardClipValR_reset() {
    this->gen_01_hardClipValR_value = 0;
}

void gen_01_hardClipValR_init() {
    this->gen_01_hardClipValR_value = 0;
}

number gen_01_superhardClipValL_getvalue() {
    return this->gen_01_superhardClipValL_value;
}

void gen_01_superhardClipValL_setvalue(number val) {
    this->gen_01_superhardClipValL_value = val;
}

void gen_01_superhardClipValL_reset() {
    this->gen_01_superhardClipValL_value = 0;
}

void gen_01_superhardClipValL_init() {
    this->gen_01_superhardClipValL_value = 0;
}

number gen_01_superhardClipValR_getvalue() {
    return this->gen_01_superhardClipValR_value;
}

void gen_01_superhardClipValR_setvalue(number val) {
    this->gen_01_superhardClipValR_value = val;
}

void gen_01_superhardClipValR_reset() {
    this->gen_01_superhardClipValR_value = 0;
}

void gen_01_superhardClipValR_init() {
    this->gen_01_superhardClipValR_value = 0;
}

number gen_01_change_4_next(number x) {
    number temp = x - this->gen_01_change_4_prev;
    this->gen_01_change_4_prev = x;
    return (temp > 0. ? 1. : (temp < 0. ? -1. : 0));
}

void gen_01_change_4_init() {
    this->gen_01_change_4_prev = 0;
}

void gen_01_change_4_reset() {
    this->gen_01_change_4_prev = 0;
}

number gen_01_change_6_next(number x) {
    number temp = x - this->gen_01_change_6_prev;
    this->gen_01_change_6_prev = x;
    return (temp > 0. ? 1. : (temp < 0. ? -1. : 0));
}

void gen_01_change_6_init() {
    this->gen_01_change_6_prev = 0;
}

void gen_01_change_6_reset() {
    this->gen_01_change_6_prev = 0;
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

void param_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_05_value;
}

void param_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_05_value_set(preset["value"]);
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
}

void param_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_07_value;
}

void param_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_07_value_set(preset["value"]);
}

void param_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_08_value;
}

void param_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_08_value_set(preset["value"]);
}

void param_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_09_value;
}

void param_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_09_value_set(preset["value"]);
}

void globaltransport_advance() {}

void globaltransport_dspsetup(bool ) {}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

Index getPatcherSerial() const {
    return 0;
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterBang(this->paramInitIndices[i], 0);
    }
}

void updateTime(MillisecondTime time, EXTERNALENGINE* engine, bool inProcess = false) {
    RNBO_UNUSED(inProcess);
    RNBO_UNUSED(engine);
    this->_currentTime = time;
    auto offset = rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr));

    if (offset >= (SampleIndex)(this->vs))
        offset = (SampleIndex)(this->vs) - 1;

    if (offset < 0)
        offset = 0;

    this->sampleOffsetIntoNextAudioBuffer = (Index)(offset);
}

void assign_defaults()
{
    p_01_target = 0;
    gen_01_in1 = 0;
    gen_01_in2 = 0;
    gen_01_clipPos = 0;
    gen_01_clipNeg = 0;
    gen_01_clipType = 0;
    gen_01_feedCoe_up = 0;
    gen_01_feedCoe_down = 0;
    gen_01_foldMode = 0;
    gen_01_Mix = 0;
    param_01_value = 0;
    param_02_value = 0;
    param_03_value = 0;
    param_04_value = 0.5;
    param_05_value = 0.5;
    param_06_value = 0;
    param_07_value = 0;
    param_08_value = 0;
    param_09_value = 0;
    ctlin_01_input = 0;
    ctlin_01_controller = 0;
    ctlin_01_channel = -1;
    expr_01_in1 = 0;
    expr_01_in2 = 0.007874015748;
    expr_01_out1 = 0;
    ctlin_02_input = 0;
    ctlin_02_controller = 0;
    ctlin_02_channel = -1;
    expr_02_in1 = 0;
    expr_02_in2 = 0.007874015748;
    expr_02_out1 = 0;
    ctlin_03_input = 0;
    ctlin_03_controller = 0;
    ctlin_03_channel = -1;
    expr_03_in1 = 0;
    expr_03_in2 = 0.007874015748;
    expr_03_out1 = 0;
    ctlin_04_input = 0;
    ctlin_04_controller = 0;
    ctlin_04_channel = -1;
    expr_04_in1 = 0;
    expr_04_in2 = 0.007874015748;
    expr_04_out1 = 0;
    ctlin_05_input = 0;
    ctlin_05_controller = 0;
    ctlin_05_channel = -1;
    expr_05_in1 = 0;
    expr_05_in2 = 0.007874015748;
    expr_05_out1 = 0;
    ctlin_06_input = 0;
    ctlin_06_controller = 0;
    ctlin_06_channel = -1;
    expr_06_in1 = 0;
    expr_06_in2 = 0.007874015748;
    expr_06_out1 = 0;
    ctlin_07_input = 0;
    ctlin_07_controller = 0;
    ctlin_07_channel = -1;
    expr_07_in1 = 0;
    expr_07_in2 = 0.007874015748;
    expr_07_out1 = 0;
    ctlin_08_input = 0;
    ctlin_08_controller = 0;
    ctlin_08_channel = -1;
    expr_08_in1 = 0;
    expr_08_in2 = 0.007874015748;
    expr_08_out1 = 0;
    ctlin_09_input = 0;
    ctlin_09_controller = 0;
    ctlin_09_channel = -1;
    expr_09_in1 = 0;
    expr_09_in2 = 0.007874015748;
    expr_09_out1 = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.000022675736961451248;
    gen_01_softClipValL_value = 0;
    gen_01_softClipValR_value = 0;
    gen_01_hardClipValL_value = 0;
    gen_01_hardClipValR_value = 0;
    gen_01_superhardClipValL_value = 0;
    gen_01_superhardClipValR_value = 0;
    gen_01_change_4_prev = 0;
    gen_01_change_6_prev = 0;
    param_01_lastValue = 0;
    param_02_lastValue = 0;
    param_03_lastValue = 0;
    param_04_lastValue = 0;
    param_05_lastValue = 0;
    param_06_lastValue = 0;
    param_07_lastValue = 0;
    param_08_lastValue = 0;
    param_09_lastValue = 0;
    ctlin_01_status = 0;
    ctlin_01_byte1 = -1;
    ctlin_01_inchan = 0;
    ctlin_02_status = 0;
    ctlin_02_byte1 = -1;
    ctlin_02_inchan = 0;
    ctlin_03_status = 0;
    ctlin_03_byte1 = -1;
    ctlin_03_inchan = 0;
    ctlin_04_status = 0;
    ctlin_04_byte1 = -1;
    ctlin_04_inchan = 0;
    ctlin_05_status = 0;
    ctlin_05_byte1 = -1;
    ctlin_05_inchan = 0;
    ctlin_06_status = 0;
    ctlin_06_byte1 = -1;
    ctlin_06_inchan = 0;
    ctlin_07_status = 0;
    ctlin_07_byte1 = -1;
    ctlin_07_inchan = 0;
    ctlin_08_status = 0;
    ctlin_08_byte1 = -1;
    ctlin_08_inchan = 0;
    ctlin_09_status = 0;
    ctlin_09_byte1 = -1;
    ctlin_09_inchan = 0;
    globaltransport_tempo = nullptr;
    globaltransport_state = nullptr;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number p_01_target;
    number gen_01_in1;
    number gen_01_in2;
    number gen_01_clipPos;
    number gen_01_clipNeg;
    number gen_01_clipType;
    number gen_01_feedCoe_up;
    number gen_01_feedCoe_down;
    number gen_01_foldMode;
    number gen_01_Mix;
    number param_01_value;
    number param_02_value;
    number param_03_value;
    number param_04_value;
    number param_05_value;
    number param_06_value;
    number param_07_value;
    number param_08_value;
    number param_09_value;
    number ctlin_01_input;
    number ctlin_01_controller;
    number ctlin_01_channel;
    number expr_01_in1;
    number expr_01_in2;
    number expr_01_out1;
    number ctlin_02_input;
    number ctlin_02_controller;
    number ctlin_02_channel;
    number expr_02_in1;
    number expr_02_in2;
    number expr_02_out1;
    number ctlin_03_input;
    number ctlin_03_controller;
    number ctlin_03_channel;
    number expr_03_in1;
    number expr_03_in2;
    number expr_03_out1;
    number ctlin_04_input;
    number ctlin_04_controller;
    number ctlin_04_channel;
    number expr_04_in1;
    number expr_04_in2;
    number expr_04_out1;
    number ctlin_05_input;
    number ctlin_05_controller;
    number ctlin_05_channel;
    number expr_05_in1;
    number expr_05_in2;
    number expr_05_out1;
    number ctlin_06_input;
    number ctlin_06_controller;
    number ctlin_06_channel;
    number expr_06_in1;
    number expr_06_in2;
    number expr_06_out1;
    number ctlin_07_input;
    number ctlin_07_controller;
    number ctlin_07_channel;
    number expr_07_in1;
    number expr_07_in2;
    number expr_07_out1;
    number ctlin_08_input;
    number ctlin_08_controller;
    number ctlin_08_channel;
    number expr_08_in1;
    number expr_08_in2;
    number expr_08_out1;
    number ctlin_09_input;
    number ctlin_09_controller;
    number ctlin_09_channel;
    number expr_09_in1;
    number expr_09_in2;
    number expr_09_out1;
    MillisecondTime _currentTime;
    ENGINE _internalEngine;
    UInt64 audioProcessSampleCount;
    Index sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[2];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number gen_01_softClipValL_value;
    number gen_01_softClipValR_value;
    number gen_01_hardClipValL_value;
    number gen_01_hardClipValR_value;
    number gen_01_superhardClipValL_value;
    number gen_01_superhardClipValR_value;
    number gen_01_change_4_prev;
    number gen_01_change_6_prev;
    number param_01_lastValue;
    number param_02_lastValue;
    number param_03_lastValue;
    number param_04_lastValue;
    number param_05_lastValue;
    number param_06_lastValue;
    number param_07_lastValue;
    number param_08_lastValue;
    number param_09_lastValue;
    Int ctlin_01_status;
    Int ctlin_01_byte1;
    Int ctlin_01_inchan;
    Int ctlin_02_status;
    Int ctlin_02_byte1;
    Int ctlin_02_inchan;
    Int ctlin_03_status;
    Int ctlin_03_byte1;
    Int ctlin_03_inchan;
    Int ctlin_04_status;
    Int ctlin_04_byte1;
    Int ctlin_04_inchan;
    Int ctlin_05_status;
    Int ctlin_05_byte1;
    Int ctlin_05_inchan;
    Int ctlin_06_status;
    Int ctlin_06_byte1;
    Int ctlin_06_inchan;
    Int ctlin_07_status;
    Int ctlin_07_byte1;
    Int ctlin_07_inchan;
    Int ctlin_08_status;
    Int ctlin_08_byte1;
    Int ctlin_08_inchan;
    Int ctlin_09_status;
    Int ctlin_09_byte1;
    Int ctlin_09_inchan;
    signal globaltransport_tempo;
    signal globaltransport_state;
    number stackprotect_count;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_04 p_01;
    bool _isInitialized = false;
};

static PatcherInterface* creaternbomatic()
{
    return new rnbomatic<EXTERNALENGINE>();
}

#ifndef RNBO_NO_PATCHERFACTORY
extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction()
#else
extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction()
#endif
{
    return creaternbomatic;
}

#ifndef RNBO_NO_PATCHERFACTORY
extern "C" void SetLogger(Logger* logger)
#else
void rnbomaticSetLogger(Logger* logger)
#endif
{
    console = logger;
}

} // end RNBO namespace

