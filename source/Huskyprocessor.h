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

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "HuskyParams.h"
#include "HuskySinging.h"
#include "HuskyLearning.h"


namespace shiroyoru {

//------------------------------------------------------------------------
//  RedHuskyProcessor
//------------------------------------------------------------------------
class RedHuskyProcessor : public Steinberg::Vst::AudioEffect
{
public:
	RedHuskyProcessor ();
	~RedHuskyProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new RedHuskyProcessor; 
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	void huskyChangeTask(bool sing) {
		if (isSinging == sing) return;
		if (sing) {
			computer.clearAllTask();
		}
		else {
			sheetMusic.clearAllNotes();
		}
	}
	Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;
//------------------------------------------------------------------------
protected:
	//float testPhase = 0.f;//this parameter should not be used in real plugin
	bool isSinging = true;
	bool bypass = false;
	Husky::SheetMusic sheetMusic = Husky::SheetMusic();
	Husky::Computer computer = Husky::Computer();
};

//------------------------------------------------------------------------
} // namespace shiroyoru
