//------------------------------------------------------------------------
// Copyright(c) 2025 shiroyoru.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------

#include "Huskyprocessor.h"
#include "Huskycids.h"
#include "HuskyIBStream.h"


#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "public.sdk/source/vst/hosting/eventlist.h"

#include "pluginterfaces/base/funknownimpl.h"
#include "pluginterfaces/base/ibstream.h"

#include <filesystem>
#include <fstream>
extern "C"{
	#include "tinyfiledialogs.h"
}


#include <memory>
#include <thread>

using namespace Steinberg;

namespace {
	const char* patterns = "*.redhusky";
	bool locked = false; //To ensure only one dialog is opened
}

namespace shiroyoru {
	namespace Husky {
		void sendUpdateMessage(RedHuskyProcessor* processor, void* source) {
			char* ptr = new char[huskyFileLength];
			memcpy(ptr, source, huskyFileLength);
			std::thread th([=]() {
				std::unique_ptr<char[]> data(ptr);
				auto* message = processor->allocateMessage();
				message->setMessageID("update");
				const void* ptr = data.get();
				
				message->getAttributes()->setBinary("values", ptr, huskyFileLength);
				auto r = processor->sendMessage(message);
				});
			th.detach();
		}
		void load(RedHuskyProcessor* processor) {
			if (locked) return;
			else locked = true;
			std::thread th([=]() {
				std::filesystem::path newFilePath = std::filesystem::current_path().append("");
				auto cstr = tinyfd_openFileDialog("Load a preset as state", newFilePath.string().c_str(), 1, &patterns, "Husky's vstpreset", 0);
				if (cstr != nullptr) {
					if (std::filesystem::path(cstr).extension().string() != std::string(".redhusky")) {
						tinyfd_messageBox("Error", "This is not the right file", "ok", "warning", 1);
						locked = false;
						return;
					}
					auto stream = Husky::StreamInterface(huskyFileLength);
					{
						std::ifstream presetFile(cstr, std::ios::binary);
						if (!presetFile.is_open()) {
							tinyfd_messageBox("Error", "Husky cannot open the file.", "ok", "warning", 1);
							locked = false;
							return;
						}
						presetFile.seekg(0, presetFile.end);
						if (presetFile.tellg() != huskyFileLength) {
							tinyfd_messageBox("Error", "Husky cannot read the file.", "ok", "warning", 1);
							locked = false;
							return;
						}
						presetFile.seekg(0, presetFile.beg);
						presetFile.read(stream.getPtr(), huskyFileLength);
						presetFile.close();
					}
					
					stream.wind();
					processor->setState(&stream) ;
					sendUpdateMessage(processor, stream.getPtr());
					//below is used to debug
					//auto tmp = tinyfd_messageBox("message", "str start", "ok", "info", 1);
					//end debug code
				}
				locked = false;
				});
			th.detach();
		}
		void save(RedHuskyProcessor* processor) {
			if (locked) return;
			else locked = true;
			std::thread th([=]() {
				std::filesystem::path newFilePath = std::filesystem::current_path().append("NewPeset.redhusky");
				// Ask the user for a preset name
				auto cstr = tinyfd_saveFileDialog("Save recent state as preset", newFilePath.string().c_str(), 1, &patterns, "Husky's vstpreset");
				if (cstr != nullptr) {// if a valid preset name exists
					auto presetName = std::filesystem::path(cstr);
					presetName.replace_extension(".redhusky");
					auto stream = Husky::StreamInterface(huskyFileLength);
					const auto getStateResult = processor->getState(&stream);
					if (getStateResult != kResultTrue) {
						tinyfd_messageBox("Error", "Husky cannot get the state.", "ok", "warning", 1);
						locked = false;
						return;
					}
					{
						std::ofstream presetFile(presetName.c_str(), std::ios::binary);
						if (!presetFile.is_open()) {
							tinyfd_messageBox("Error", "Husky is not allowed to save the file.", "ok", "warning", 1);
							locked = false;
							return;
						}
						presetFile.write(stream.getPtr(),huskyFileLength);
						presetFile.close();
						//below is used to debug
						//const float* float_ptr = (float*)stream.getPtr();
						//auto str = std::string("str start");
						//for (unsigned i = 0; i < 94; ++i) {
						//	str = str + std::to_string(float_ptr[i]) + ",";
						//}
						//auto tmp = tinyfd_messageBox("message", str.c_str(), "ok", "info", 1);
						//end debug code
					}
				}
				locked = false;
				});
			th.detach();
		}
		inline float clip(float f) {
			if (std::isfinite(f)) {
				return((f > 0.f) ? ((f < 1.f) ? f : 1.f) : 0.f);
			}
			else {
				return 0.f;
			}
		}
	}
//------------------------------------------------------------------------
// RedHuskyProcessor
//------------------------------------------------------------------------
RedHuskyProcessor::RedHuskyProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kRedHuskyControllerUID);
}

//------------------------------------------------------------------------
RedHuskyProcessor::~RedHuskyProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------
	using namespace Husky;
	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData(index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount();
				paramQueue->getPoint(numPoints - 1, sampleOffset, value);
				int x = paramQueue->getParameterId();
				int type = x >> typeIndexLeftShift;
				int nth = (x & nthMask);
				switch (type)
				{
				case vControllerIndex:
					sheetMusic.setHarmonics(nth, vIndex, value);// a work around
					break;
				case aIndex:
				case dIndex:
				case sIndex:
				case rIndex:
					sheetMusic.setHarmonics(nth, type, value);
					break;
				case keyIndex:
					sheetMusic.setKeyAdjust(nth, value);
					break;
				case fqMainIndex:
					sheetMusic.setFqMain(value);
					computer.setFrequency(value);
					break;
				case taskIndex:
					isSinging = (value > 0.5f);
					break;
				case bypassIndex:
					bypass = (value > 0.5);
					break;
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	Vst::IEventList* events = data.inputEvents;
	if (data.numSamples > 0)
	{	
		if (!bypass) {
			if (isSinging) {
				//if (data.numOutputs <= 0)return kResultOk;
				if (events != NULL) {
					int32 numEvents = events->getEventCount();
					for (int32 i = 0; i < numEvents; i++) {
						Vst::Event event;
						if (events->getEvent(i, event) == kResultOk) {
							switch (event.type) {
							case Vst::Event::kNoteOnEvent:
								sheetMusic.addNote(event.noteOn.noteId, event.noteOn.pitch, event.noteOn.velocity, data.processContext->sampleRate);
								break;
							case Vst::Event::kNoteOffEvent:
								sheetMusic.releaseNote(event.noteOff.noteId, event.noteOff.pitch);//noteId not working
								break;
							}
						}
					}
				}
				if (data.numOutputs == 0)return kResultOk;
				Vst::Sample32** out = data.outputs[0].channelBuffers32;
				sheetMusic.updateSamples(out, data.numSamples, data.outputs[0].numChannels);
				return kResultOk;
			}
			else {
				if (data.numInputs <= 0 || data.inputs[0].numChannels <= 0)return kResultOk;
				if (events != NULL) {
					int32 numEvents = events->getEventCount();
					for (int32 i = 0; i < numEvents; i++) {
						Vst::Event event;
						if (events->getEvent(i, event) == kResultOk) {
							switch (event.type) {
							case Vst::Event::kNoteOnEvent:
								computer.recognizeNoteDown(event.noteOn.pitch, data.processContext->sampleRate);
								break;
							case Vst::Event::kNoteOffEvent:
								computer.recognizeNoteRelease();
								break;
							}
						}
					}
				}
				Vst::Sample32** in = data.inputs[0].channelBuffers32;
				computer.listen(in, data.numSamples);
				/*
				Vst::Sample32** out = data.outputs[0].channelBuffers32;
				for (int c = 0; c < data.outputs[0].numChannels; ++c) {
					memset(out[c], 0, sizeof(float)*data.numSamples);
				}
				*/
				return kResultOk;
			}
		}
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::setState (IBStream* state)
{
	using namespace Husky;
	IBStreamer streamer(state, kLittleEndian);
	float fVal;
	for (int n = 0; n < numHarmonics; ++n) {
		for (int t = 0; t < 5; ++t) {
			if (streamer.readFloat(fVal) == false)
				return kResultFalse;
			sheetMusic.setHarmonics(n, t, fVal);
		}
	}
	for (int k = 0; k < 12; ++k) {
		if (streamer.readFloat(fVal) == false)
			return kResultFalse;
		sheetMusic.setKeyAdjust(k, fVal);
	}
	if (streamer.readFloat(fVal) == false)
		return kResultFalse;
	sheetMusic.setFqMain(fVal);
	computer.setFrequency(fVal);
	if (streamer.readFloat(fVal) == false)
		return kResultFalse;
	bypass = fVal > 0.5f ? true : false;
	//debug
	//std::thread([]() {
	//	tinyfd_messageBox("Information", "setState called.", "ok", "info", 1);
	//	}).detach();
	//debug end
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API RedHuskyProcessor::getState (IBStream* state)
{
	using namespace Husky;
	// here we need to save the model
	IBStreamer streamer(state, kLittleEndian);
	auto saveLearningResult = false;
	if (!isSinging) {
		saveLearningResult = computer.conclude();
	}
	if (saveLearningResult) {
		StreamInterface sInterface(Husky::huskyFileLength);
		IBStreamer mStreamer(&sInterface, kLittleEndian);
		for (int n = 0; n < numHarmonics; ++n) {
			for (int t = 0; t < 5; ++t) {
				const float resultNT = clip(computer.getResult(n, t));
				streamer.writeFloat(resultNT);
				mStreamer.writeFloat(resultNT);
				sheetMusic.setHarmonics(n, t, resultNT);
			}
		}
		for (int k = 0; k < 12; ++k) {
			const float kHeight = clip(sheetMusic.getKeyAdjust(k));
			streamer.writeFloat(kHeight);
			mStreamer.writeFloat(kHeight);
		}
		const float fqMain = clip(sheetMusic.getFqMain());
		streamer.writeFloat(fqMain);
		mStreamer.writeFloat(fqMain);
		const float b = bypass ? 1.f : 0.f;
		streamer.writeFloat(b);
		mStreamer.writeFloat(b);
		isSinging = true;
		Husky::sendUpdateMessage(this,sInterface.getPtr());
	}
	else {
		for (int n = 0; n < numHarmonics; ++n) {
			for (int t = 0; t < 5; ++t) {
				const float h = clip(sheetMusic.getHarmonics(n, t));
				streamer.writeFloat(h);
			}
		}
		for (int k = 0; k < 12; ++k) {
			const float kHeight = clip(sheetMusic.getKeyAdjust(k));
			streamer.writeFloat(kHeight);
		}
		const float fqMain = clip(sheetMusic.getFqMain());
		streamer.writeFloat(fqMain);
		const float b = bypass ? 1.f : 0.f;
		streamer.writeFloat(b);
	}
	return kResultOk;
}

tresult PLUGIN_API RedHuskyProcessor::notify(Steinberg::Vst::IMessage* message) {
	if (message) {
		
		if (std::string(message->getMessageID()) == std::string("0x800")) {
			Husky::load(this);
		}
		else if (std::string(message->getMessageID()) == std::string("0x801")) {
			Husky::save(this);
		}
	}
	return kResultOk;
}
//------------------------------------------------------------------------
} // namespace shiroyoru
