//------------------------------------------------------------------------
// Copyright(c) 2025 shiroyoru.
//
// This program is free software : you can redistribute it and /or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see < https://www.gnu.org/licenses/>.
// 
//------------------------------------------------------------------------

#include "Huskycontroller.h"
#include "Huskycids.h"
#include "HuskyParams.h"
#include "HuskyEditor.h"
#include "HuskyIBStream.h"

#include "pluginterfaces/base/ustring.h"
#include "base/source/fstring.h"
#include "pluginterfaces/base/ibstream.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include <filesystem>
#include <fstream>

using namespace Steinberg;

namespace shiroyoru {
	namespace Husky {
		class VSParameter : public Vst::Parameter {
		public:
			VSParameter(const Vst::ParameterInfo& info) :Parameter(info) {}
			void toString(Vst::ParamValue normValue, Vst::String128 string) const SMTG_OVERRIDE {
				char text[32];
				snprintf(text, 32, "%.2f", (float)(maxShowVolume * normValue));
				Steinberg::UString(string, 128).fromAscii(text);
			}
			bool fromString(const Vst::TChar* string, Vst::ParamValue& normValue)const SMTG_OVERRIDE {
				String wrapper((Vst::TChar*)string); // don't know buffer size here!
				double tmp = 0.0;
				if (wrapper.scanFloat(tmp))
				{
					// allow only values between 1 and 10000
					if (tmp < 0.0)
					{
						tmp = 0.0;
					}
					else if (tmp > maxShowVolume) {
						tmp = maxShowVolume;
					}
					normValue = 1.f / maxShowVolume * tmp;
					return true;
				}
				return false;
			}
		};
		class ADRParameter : public Vst::Parameter {
		public:
			ADRParameter(const Vst::ParameterInfo& info) :Parameter(info) {}
			void toString(Vst::ParamValue normValue, Vst::String128 string) const SMTG_OVERRIDE {
				char text[32];
				snprintf(text, 32, "%d", (int)(maxTimeMiliseconds * normValue));
				Steinberg::UString(string, 128).fromAscii(text);
			}
			bool fromString(const Vst::TChar* string, Vst::ParamValue& normValue)const SMTG_OVERRIDE {
				String wrapper((Vst::TChar*)string); // don't know buffer size here!
				double tmp = 0.0;
				if (wrapper.scanFloat(tmp))
				{
					// allow only values between 1 and 10000
					if (tmp < 0.0)
					{
						tmp = 0.0;
					}
					else if (tmp > maxTimeMiliseconds) {
						tmp = maxTimeMiliseconds;
					}
					normValue = 1.f / maxTimeMiliseconds * tmp;
					return true;
				}
				return false;
			}
		};
		class KeyParameter : public  Vst::Parameter {
		public:
			KeyParameter(const Vst::ParameterInfo& info) :Parameter(info) {}
			void toString(Vst::ParamValue normValue, Vst::String128 string) const SMTG_OVERRIDE {
				char text[32];
				snprintf(text, 32, "%.4f", (float)(normValue + 1.f));
				Steinberg::UString(string, 128).fromAscii(text);
			}
			bool fromString(const Vst::TChar* string, Vst::ParamValue& normValue)const SMTG_OVERRIDE {
				String wrapper((Vst::TChar*)string); // don't know buffer size here!
				double tmp = 0.0;
				if (wrapper.scanFloat(tmp))
				{
					// allow only values between 1 and 10000
					if (tmp < 1.0)
					{
						tmp = 1.0;
					}
					else if (tmp > 2.0) {
						tmp = 2.0;
					}
					normValue = tmp - 1.f;
					return true;
				}
				return false;
			}
		};
		class FqMainParameter : public Vst::Parameter {
		public:
			FqMainParameter(const Vst::ParameterInfo& info) :Parameter(info) {}
			void toString(Vst::ParamValue normValue, Vst::String128 string) const SMTG_OVERRIDE {
				char text[32];
				snprintf(text, 32, "%.1f", (float)(normValue * (maxFq - minFq) + minFq));
				Steinberg::UString(string, 128).fromAscii(text);
			}
			bool fromString(const Vst::TChar* string, Vst::ParamValue& normValue)const SMTG_OVERRIDE {
				String wrapper((Vst::TChar*)string); // don't know buffer size here!
				double tmp = 0.0;
				if (wrapper.scanFloat(tmp))
				{
					// allow only values between 1 and 10000
					if (tmp < minFq)
					{
						tmp = minFq;
					}
					else if (tmp > maxFq) {
						tmp = maxFq;
					}
					normValue = 1.f / (maxFq - minFq) * (tmp - minFq);
					return true;
				}
				return false;
			}
		};
	}


//------------------------------------------------------------------------
// RedHuskyController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	using namespace Husky;
	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters

	setKnobMode(Vst::kLinearMode);

	auto c = [](unsigned n, unsigned typeIndex, const char* typeName, float value) {
		Vst::ParameterInfo pI = Vst::ParameterInfo();
		UString128((typeName + std::to_string(n)).c_str()).copyTo(pI.title, 128);
		pI.flags = Vst::ParameterInfo::kCanAutomate;
		pI.stepCount = 0;
		pI.id = (typeIndex << typeIndexLeftShift) + n;
		pI.defaultNormalizedValue = value;
		return pI;
		};
	parameters.addParameter(new VSParameter(c(0, vControllerIndex, "volume ", 1.f)));
	parameters.addParameter(new ADRParameter(c(0, aIndex, "attack ", 0.001f)));
	parameters.addParameter(new ADRParameter(c(0, dIndex, "decay ", 0.01f)));
	parameters.addParameter(new VSParameter(c(0, sIndex, "sustain ", 0.5f)));
	parameters.addParameter(new ADRParameter(c(0, rIndex, "release ", 0.001f)));
	for (unsigned n = 1; n < numHarmonics; ++n) {
		parameters.addParameter(new VSParameter(c(n, vControllerIndex, "volume ", 0.f)));
		parameters.addParameter(new ADRParameter(c(n, aIndex, "attack ", 0.001f)));
		parameters.addParameter(new ADRParameter(c(n, dIndex, "decay ", 0.01f)));
		parameters.addParameter(new VSParameter(c(n, sIndex, "sustain ", 0.5f)));
		parameters.addParameter(new ADRParameter(c(n, rIndex, "release ",0.001f)));
	}
	for (unsigned k = 0; k < 12; ++k) {
		parameters.addParameter(new KeyParameter(c(k, keyIndex, "key ", powf(2.f, (float)k / 12) - 1.f)));
	}
	parameters.addParameter(new FqMainParameter(c(0, fqMainIndex, "main frequency", norm440hz)));

	int32 flags = Vst::ParameterInfo::kCanAutomate;
	parameters.addParameter(u"husky's task", nullptr, 1, 1.f, flags, taskIndex << typeIndexLeftShift);

	//parameters.addParameter(u"innerUpdate", nullptr, 1, 0.f, Vst::ParameterInfo::kIsReadOnly, (saveIndex << typeIndexLeftShift) + 2);

	flags |= Vst::ParameterInfo::kIsBypass;
	parameters.addParameter(u"bypass", nullptr, 1, 0, flags, bypassIndex << typeIndexLeftShift);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	using namespace Husky;

	//start inner update
	//setParamNormalized((saveIndex << typeIndexLeftShift) + 2, 1.f);

	IBStreamer streamer(state, kLittleEndian);

	float fVal;
	for (int n = 0; n < numHarmonics; ++n) {
		for (int t = 0; t < 5; ++t) {
			if (streamer.readFloat(fVal) == false) {
				return kResultFalse;
			}
			if (t) {//another work around, this is why I attemped to use 0 as tag
				setParamNormalized((t << typeIndexLeftShift) + n, fVal);
			}
			else {
				setParamNormalized((vControllerIndex << typeIndexLeftShift) + n, fVal);
			}
		}
	}
	for (int k = 0; k < 12; ++k) {
		if (streamer.readFloat(fVal) == false)
			return kResultFalse;
		setParamNormalized((keyIndex << typeIndexLeftShift) + k, fVal);
	}
	if (streamer.readFloat(fVal) == false)
		return kResultFalse;
	setParamNormalized(fqMainIndex << typeIndexLeftShift, fVal);
	if (streamer.readFloat(fVal) == false)
		return kResultFalse;
	setParamNormalized(bypassIndex << typeIndexLeftShift, fVal);
	setParamNormalized(taskIndex << typeIndexLeftShift, 1.f);

	//end inner update
	//setParamNormalized((saveIndex << typeIndexLeftShift) + 2, 0.f);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API RedHuskyController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new Husky::CustomEditor(this, "view", "Huskyeditor.uidesc");
		return view;
	}
	return nullptr;
}

tresult PLUGIN_API RedHuskyController::notify(Steinberg::Vst::IMessage* message) {
	if (message) {
		if (std::string(message->getMessageID()) == std::string("update")) {
			const void* stateData = nullptr;
			uint32 stateSize = 0;
			if (message->getAttributes()->getBinary("values", stateData, stateSize) == kResultOk)
			{
				//tinyfd_messageBox("message", "The file successfully loaded", "ok", "info", 1);
				using namespace Husky;
				if (stateSize != huskyFileLength) return kResultFalse;

				//start inner update
				//setParamNormalized((saveIndex << typeIndexLeftShift) + 2, 1.f);

				const float* values = (const float*)stateData;
				for (int n = 0; n < numHarmonics; ++n) {
					setParamNormalized((vControllerIndex << typeIndexLeftShift) + n, values[vIndex]);
					setParamNormalized((aIndex << typeIndexLeftShift) + n, values[aIndex]);
					setParamNormalized((dIndex << typeIndexLeftShift) + n, values[dIndex]);
					setParamNormalized((sIndex << typeIndexLeftShift) + n, values[sIndex]);
					setParamNormalized((rIndex << typeIndexLeftShift) + n, values[rIndex]);
					values += 5;
				}
				for (int k = 0; k < 12; ++k) {
					setParamNormalized((keyIndex << typeIndexLeftShift) + k, values[k]);
				}
				setParamNormalized(fqMainIndex << typeIndexLeftShift, values[12]);
				setParamNormalized(bypassIndex << typeIndexLeftShift, values[13]);
				setParamNormalized(taskIndex << typeIndexLeftShift, 1.f);

				//end inner update
				//setParamNormalized((saveIndex << typeIndexLeftShift) + 2, 0.f);
			}
		}
	}
	return kResultOk;
}

} // namespace shiroyoru
