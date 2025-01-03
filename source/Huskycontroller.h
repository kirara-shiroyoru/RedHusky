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

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace shiroyoru {

//------------------------------------------------------------------------
//  RedHuskyController
//------------------------------------------------------------------------
class RedHuskyController : public Steinberg::Vst::EditControllerEx1
{
public:
//------------------------------------------------------------------------
	RedHuskyController () = default;
	~RedHuskyController () SMTG_OVERRIDE = default;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/)
	{
		return (Steinberg::Vst::IEditController*)new RedHuskyController;
	}

	//--- from IPluginBase -----------------------------------------------
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	//--- from EditController --------------------------------------------
	Steinberg::tresult PLUGIN_API setComponentState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::IPlugView* PLUGIN_API createView (Steinberg::FIDString name) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;

 	//---Interface---------
	DEFINE_INTERFACES
	// Here you can add more supported VST3 interfaces
	// DEF_INTERFACE (Vst::IXXX)
	END_DEFINE_INTERFACES(EditController)
	DELEGATE_REFCOUNT(EditController)


	//---------defined function for processor communicating----------------------

	void load();

//------------------------------------------------------------------------
protected:
};

//------------------------------------------------------------------------
} // namespace shiroyoru
