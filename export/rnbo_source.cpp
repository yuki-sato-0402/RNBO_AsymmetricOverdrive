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

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define trunc(x) ((Int)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_04 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_04()
    {
    }
    
    ~RNBOSubpatcher_04()
    {
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
    
    SampleIndex currentsampletime() {
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
        this->updateTime(this->getEngine()->getCurrentTime());
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
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            {
            this->setProbingIndex(-1);
            break;
            }
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
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
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
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
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
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
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
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
    
    void eventinlet_01_out1_list_set(const list& v) {
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
    
    void eventinlet_02_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_04_value_set(converted);
        }
    }
    
    void linetilde_01_target_bang() {}
    
    void linetilde_02_target_bang() {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
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
    
    Index getNumInputChannels() const {
        return 2;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->numberobj_01_init();
        this->numberobj_02_init();
        this->numberobj_03_init();
        this->numberobj_04_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void linetilde_01_time_set(number v) {
        this->linetilde_01_time = v;
    }
    
    void linetilde_01_segments_set(const list& v) {
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
            list converted = {v};
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
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Pre-EQ~/number_obj-11"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_01_output_set(localvalue);
    }
    
    void scale_01_out_set(const list& v) {
        this->scale_01_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_01_value_set(converted);
        }
    }
    
    void scale_01_input_set(const list& v) {
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
            list converted = {v};
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
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Pre-EQ~/number_obj-3"), localvalue, this->_currentTime);
        this->numberobj_02_output_set(localvalue);
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void linetilde_02_time_set(number v) {
        this->linetilde_02_time = v;
    }
    
    void linetilde_02_segments_set(const list& v) {
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
            list converted = {v};
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
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Pre-EQ~/number_obj-29"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_03_output_set(localvalue);
    }
    
    void scale_02_out_set(const list& v) {
        this->scale_02_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_03_value_set(converted);
        }
    }
    
    void scale_02_input_set(const list& v) {
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
            list converted = {v};
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
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Pre-EQ~/number_obj-33"),
            localvalue,
            this->_currentTime
        );
    
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
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Pre-EQ~/number_obj-3"), localvalue, this->_currentTime);
        this->numberobj_02_output_set(localvalue);
    }
    
    void numberobj_04_value_bang() {
        number v = this->numberobj_04_value;
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Pre-EQ~/number_obj-33"),
            localvalue,
            this->_currentTime
        );
    
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
    
        for (i = 0; i < n; i++) {
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
    
        for (i = 0; i < n; i++) {
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
    
        for (i = 0; i < n; i++) {
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
    
        for (i = 0; i < n; i++) {
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
    
        for (i = 0; i < n; i++) {
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
    
        for (i = 0; i < n; i++) {
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Pre-EQ~/number_obj-11"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Pre-EQ~/number_obj-3"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Pre-EQ~/number_obj-29"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Pre-EQ~/number_obj-33"), 1, this->_currentTime);
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
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
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
        linetilde_01_keepramp = 0;
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
        linetilde_02_keepramp = 0;
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
        invsr = 0.00002267573696;
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
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
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
    
};

rnbomatic()
{
}

~rnbomatic()
{
    delete this->p_01;
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
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

template <typename T> void listswapelements(T& arr, Int a, Int b) {
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

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo(this->currenttime());
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
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
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    this->p_01_perform(in1, in2, this->signals[0], this->signals[1], n);

    this->gen_01_perform(
        this->signals[0],
        this->signals[1],
        this->gen_01_Mix,
        this->gen_01_clipType,
        this->gen_01_clipNeg,
        this->gen_01_clipPos,
        out1,
        out2,
        n
    );

    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
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

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        {
        this->setProbingIndex(-1);
        break;
        }
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
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

void fillDataRef(DataRefIndex , DataRef& ) {}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->p_01->processDataViewUpdate(index, time);
}

void initialize() {
    this->assign_defaults();
    this->setState();
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {
    this->p_01 = new RNBOSubpatcher_04();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "preLowcut"));
    this->param_02_getPresetValue(getSubState(preset, "preHighcut"));
    this->param_03_getPresetValue(getSubState(preset, "Mix"));
    this->param_04_getPresetValue(getSubState(preset, "clipType"));
    this->param_05_getPresetValue(getSubState(preset, "clipNeg"));
    this->param_06_getPresetValue(getSubState(preset, "clipPos"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "Pre-EQ~"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "preLowcut"));
    this->param_02_setPresetValue(getSubState(preset, "preHighcut"));
    this->param_03_setPresetValue(getSubState(preset, "Mix"));
    this->param_04_setPresetValue(getSubState(preset, "clipType"));
    this->param_05_setPresetValue(getSubState(preset, "clipNeg"));
    this->param_06_setPresetValue(getSubState(preset, "clipPos"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false)) {
        this->p_01->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false)) {
        this->p_01->processTimeSignatureEvent(time, numerator, denominator);
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

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
    default:
        {
        index -= 6;

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
    default:
        {
        index -= 6;

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
    return 6 + this->p_01->getNumParameters();
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
        return "Mix";
        }
    case 3:
        {
        return "clipType";
        }
    case 4:
        {
        return "clipNeg";
        }
    case 5:
        {
        return "clipPos";
        }
    default:
        {
        index -= 6;

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
        return "Mix";
        }
    case 3:
        {
        return "clipType";
        }
    case 4:
        {
        return "clipNeg";
        }
    case 5:
        {
        return "clipPos";
        }
    default:
        {
        index -= 6;

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
        case 3:
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
        case 4:
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
        default:
            {
            index -= 6;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 6;

    return 0;
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
    case 3:
        {
        {
            value = (value < 0 ? 0 : (value > 2 ? 2 : value));
            ParameterValue normalizedValue = (value - 0) / (2 - 0);
            return normalizedValue;
        }
        }
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
        {
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 3:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (2 - 0);
            }
        }
        }
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (100 - 0);
            }
        }
        }
    default:
        {
        index -= 6;

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
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        return value;
        }
    }
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

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
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

    this->gen_01_Mix_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->gen_01_clipType_set(v);
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    this->gen_01_clipNeg_set(v);
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->gen_01_clipPos_set(v);
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
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

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    this->p_01->allocateDataRefs();
}

void initializeObjects() {
    this->p_01->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
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

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in3_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_01_out1_number_set(v);
}

static number param_02_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in4_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_02_out1_number_set(v);
}

static number param_03_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_Mix_set(number v) {
    this->gen_01_Mix = v;
}

static number param_04_value_constrain(number v) {
    v = (v > 2 ? 2 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipType_set(number v) {
    this->gen_01_clipType = v;
}

static number param_05_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipNeg_set(number v) {
    this->gen_01_clipNeg = v;
}

static number param_06_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_clipPos_set(number v) {
    this->gen_01_clipPos = v;
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
    number Mix,
    number clipType,
    number clipNeg,
    number clipPos,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    Index i;

    for (i = 0; i < n; i++) {
        number mixed1L_0 = 0;
        number mixed2L_1 = 0;
        number mixed1R_2 = 0;
        number mixed2R_3 = 0;
        number softClip_4 = 0;

        {
            auto val2_7 = clipNeg;
            auto val1_6 = clipPos;
            auto input_5 = in1[(Index)i];
            number value1_8 = 0;
            number value2_9 = 0;
            auto scaledClip1_10 = this->scale(val1_6, 0, 100, 1, 10, 1);
            auto scaledClip2_11 = this->scale(val2_7, 0, 100, 1, 10, 1);
            auto scaledClip3_12 = this->scale(val1_6, 0, 100, 1.9, 1, 0.2);
            auto scaledClip4_13 = this->scale(val2_7, 0, 100, 1.9, 1, 0.2);

            if (input_5 >= 0) {
                value1_8 = scaledClip3_12 * rnbo_atan(input_5 * scaledClip1_10);
            } else {
                value2_9 = scaledClip4_13 * rnbo_atan(input_5 * scaledClip2_11);
            }

            softClip_4 = (value1_8 + value2_9) * 2 / 3.14159265358979323846;
        }

        number softClipValL_14 = softClip_4;
        number softClip_15 = 0;

        {
            auto val2_18 = clipNeg;
            auto val1_17 = clipPos;
            auto input_16 = in2[(Index)i];
            number value1_19 = 0;
            number value2_20 = 0;
            auto scaledClip1_21 = this->scale(val1_17, 0, 100, 1, 10, 1);
            auto scaledClip2_22 = this->scale(val2_18, 0, 100, 1, 10, 1);
            auto scaledClip3_23 = this->scale(val1_17, 0, 100, 1.9, 1, 0.2);
            auto scaledClip4_24 = this->scale(val2_18, 0, 100, 1.9, 1, 0.2);

            if (input_16 >= 0) {
                value1_19 = scaledClip3_23 * rnbo_atan(input_16 * scaledClip1_21);
            } else {
                value2_20 = scaledClip4_24 * rnbo_atan(input_16 * scaledClip2_22);
            }

            softClip_15 = (value1_19 + value2_20) * 2 / 3.14159265358979323846;
        }

        number softClipValR_25 = softClip_15;
        number hardClip_26 = 0;

        {
            auto val2_29 = clipNeg;
            auto val1_28 = clipPos;
            auto input_27 = in1[(Index)i];
            number value1_30 = 0;
            number value2_31 = 0;
            auto scaledClip1_32 = this->scale(val1_28, 0, 100, 1, 10, 1);
            auto scaledClip2_33 = this->scale(val2_29, 0, 100, 1, 10, 1);
            auto scaledClip3_34 = this->scale(val1_28, 0, 100, 1.25, 0.9, 0.15);
            auto scaledClip4_35 = this->scale(val2_29, 0, 100, 1.25, 0.9, 0.15);

            if (input_27 >= 0) {
                value1_30 = scaledClip3_34 * rnbo_tanh(input_27 * scaledClip1_32);
            } else {
                value2_31 = scaledClip4_35 * rnbo_tanh(input_27 * scaledClip2_33);
            }

            hardClip_26 = value1_30 + value2_31;
        }

        number hardClipValL_36 = hardClip_26;
        number hardClip_37 = 0;

        {
            auto val2_40 = clipNeg;
            auto val1_39 = clipPos;
            auto input_38 = in2[(Index)i];
            number value1_41 = 0;
            number value2_42 = 0;
            auto scaledClip1_43 = this->scale(val1_39, 0, 100, 1, 10, 1);
            auto scaledClip2_44 = this->scale(val2_40, 0, 100, 1, 10, 1);
            auto scaledClip3_45 = this->scale(val1_39, 0, 100, 1.25, 0.9, 0.15);
            auto scaledClip4_46 = this->scale(val2_40, 0, 100, 1.25, 0.9, 0.15);

            if (input_38 >= 0) {
                value1_41 = scaledClip3_45 * rnbo_tanh(input_38 * scaledClip1_43);
            } else {
                value2_42 = scaledClip4_46 * rnbo_tanh(input_38 * scaledClip2_44);
            }

            hardClip_37 = value1_41 + value2_42;
        }

        number hardClipValR_47 = hardClip_37;
        number superhardClip_48 = 0;

        {
            auto val2_51 = clipNeg;
            auto val1_50 = clipPos;
            auto input_49 = in1[(Index)i];
            auto scaledClip1_52 = this->scale(val1_50, 100, 0, 0, 1, 1);
            auto scaledClip2_53 = this->scale(val2_51, 100, 0, 0, 1, 1);
            number div5_54 = fixnan(rnbo_pow(scaledClip1_52 * 10, 2)) / (number)100;
            number div6_55 = fixnan(rnbo_pow(scaledClip2_53 * 10, 2)) / (number)100;
            number clamp8_56 = (in1[(Index)i] > div5_54 ? div5_54 : (in1[(Index)i] < -div6_55 ? -div6_55 : in1[(Index)i]));
            number div10_57 = (-div6_55 + div5_54) / (number)2;
            number div13_58 = (div5_54 + div6_55) / (number)2;
            number rdiv14_59 = (div13_58 == 0. ? 0. : (number)1 / div13_58);
            superhardClip_48 = (clamp8_56 - div10_57) * rdiv14_59 * 0.9;
        }

        number superhardClipValL_60 = superhardClip_48;
        number superhardClip_61 = 0;

        {
            auto val2_64 = clipNeg;
            auto val1_63 = clipPos;
            auto input_62 = in2[(Index)i];
            auto scaledClip1_65 = this->scale(val1_63, 100, 0, 0, 1, 1);
            auto scaledClip2_66 = this->scale(val2_64, 100, 0, 0, 1, 1);
            number div5_67 = fixnan(rnbo_pow(scaledClip1_65 * 10, 2)) / (number)100;
            number div6_68 = fixnan(rnbo_pow(scaledClip2_66 * 10, 2)) / (number)100;
            number clamp8_69 = (in1[(Index)i] > div5_67 ? div5_67 : (in1[(Index)i] < -div6_68 ? -div6_68 : in1[(Index)i]));
            number div10_70 = (-div6_68 + div5_67) / (number)2;
            number div13_71 = (div5_67 + div6_68) / (number)2;
            number rdiv14_72 = (div13_71 == 0. ? 0. : (number)1 / div13_71);
            superhardClip_61 = (clamp8_69 - div10_70) * rdiv14_72 * 0.9;
        }

        number superhardClipValR_73 = superhardClip_61;

        if (0 <= clipType && clipType <= 1) {
            mixed1L_0 = softClipValL_14 + clipType * (hardClipValL_36 - softClipValL_14);
            mixed1R_2 = softClipValR_25 + clipType * (hardClipValR_47 - softClipValR_25);
        } else {
            mixed2L_1 = hardClipValL_36 + (clipType - 1) * (superhardClipValL_60 - hardClipValL_36);
            mixed2R_3 = hardClipValR_47 + (clipType - 1) * (superhardClipValR_73 - hardClipValR_47);
        }

        number expr_1_74 = in1[(Index)i] + Mix / (number)100 * (mixed1L_0 + mixed2L_1 - in1[(Index)i]);
        number expr_2_75 = in2[(Index)i] + Mix / (number)100 * (mixed1R_2 + mixed2R_3 - in2[(Index)i]);
        out2[(Index)i] = expr_2_75;
        out1[(Index)i] = expr_1_74;
    }
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
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

Index globaltransport_getSampleOffset(MillisecondTime time) {
    return this->mstosamps(this->maximum(0, time - this->getEngine()->getCurrentTime()));
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState(MillisecondTime time) {
    return this->globaltransport_getStateAtSample(this->globaltransport_getSampleOffset(time));
}

number globaltransport_getBeatTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState(time) == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(MillisecondTime time, number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(time, tempo);
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getTempoAtSample(offset) != tempo) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
            this->globaltransport_beatTimeChanges->push(time);
            fillSignal(this->globaltransport_tempo, this->vs, tempo, offset);
            this->globaltransport_lastTempo = tempo;
            this->globaltransport_tempoNeedsReset = true;
            return true;
        }
    }

    return false;
}

number globaltransport_getTempo(MillisecondTime time) {
    return this->globaltransport_getTempoAtSample(this->globaltransport_getSampleOffset(time));
}

bool globaltransport_setState(MillisecondTime time, number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(time, TransportState(state));
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getStateAtSample(offset) != state) {
            fillSignal(this->globaltransport_state, this->vs, state, offset);
            this->globaltransport_lastState = TransportState(state);
            this->globaltransport_stateNeedsReset = true;

            if (state == 0) {
                this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
                this->globaltransport_beatTimeChanges->push(time);
            }

            return true;
        }
    }

    return false;
}

bool globaltransport_setBeatTime(MillisecondTime time, number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(time, beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime(time));
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(time);
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTime(this->getEngine()->getCurrentTime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignature(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignature(this->getEngine()->getCurrentTime() + msOffset);
}

bool globaltransport_setTimeSignature(MillisecondTime time, number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(time, (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature(time);

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(time);
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    p_01_target = 0;
    gen_01_in1 = 0;
    gen_01_in2 = 0;
    gen_01_Mix = 0;
    gen_01_clipType = 0;
    gen_01_clipNeg = 0;
    gen_01_clipPos = 0;
    param_01_value = 0;
    param_02_value = 0;
    param_03_value = 0;
    param_04_value = 0;
    param_05_value = 0;
    param_06_value = 0;
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
    invsr = 0.00002267573696;
    param_01_lastValue = 0;
    param_02_lastValue = 0;
    param_03_lastValue = 0;
    param_04_lastValue = 0;
    param_05_lastValue = 0;
    param_06_lastValue = 0;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number p_01_target;
    number gen_01_in1;
    number gen_01_in2;
    number gen_01_Mix;
    number gen_01_clipType;
    number gen_01_clipNeg;
    number gen_01_clipPos;
    number param_01_value;
    number param_02_value;
    number param_03_value;
    number param_04_value;
    number param_05_value;
    number param_06_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[2];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number param_01_lastValue;
    number param_02_lastValue;
    number param_03_lastValue;
    number param_04_lastValue;
    number param_05_lastValue;
    number param_06_lastValue;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_04* p_01;

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

