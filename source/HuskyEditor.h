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


#pragma once

#include "vstgui/plugin-bindings/vst3editor.h"
#include "HuskyParams.h"
#include <chrono>
//debug 
extern "C" {
#include "tinyfiledialogs.h"
}
#include <thread>
//end debug




namespace shiroyoru {namespace Husky {
    bool innerUpdate = false;
    class CustomEditor : public VSTGUI::VST3Editor {
        int i4 = 2;//just a patch to avoid unexpected call when vst3editor initialize
    public:
        CustomEditor(Steinberg::Vst::EditController* controller, VSTGUI::UTF8StringPtr templateName, VSTGUI::UTF8StringPtr xmlFile)
            : VST3Editor(controller, templateName, xmlFile) {}
    protected:
        void valueChanged(VSTGUI::CControl* control) override {
            const auto tag = control->getTag();
            switch (tag) {
            case (saveIndex << typeIndexLeftShift) + 0:
                if (i4 > 0) {//just a patch to avoid unexpected call when vst3editor initialize
                    --i4;
                    return;
                }
                controller->sendMessageID("0x800");
                break;
            case (saveIndex << typeIndexLeftShift) + 1:
                if (i4 > 0) {//just a patch to avoid unexpected call when vst3editor initialize
                    --i4;
                    return;
                }
                controller->sendMessageID("0x801");
                break;
                /*
            case (saveIndex << typeIndexLeftShift) + 2:
                innerUpdate = (control->getValue() > 0.5f);
                if (!innerUpdate) {
                    getFrame()->invalid();
                }
                break;
                */
            default:
                /*
                if (!innerUpdate) {
                    getController()->beginEdit(tag);
                    getController()->performEdit(tag, control->getValue());
                    getController()->endEdit(tag);
                    control->invalid();
                }
                */ 
                VSTGUI::VST3Editor::valueChanged(control);
                break;
            }
        }
    };
}}



