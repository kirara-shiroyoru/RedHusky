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

#include "vstgui/plugin-bindings/vst3editor.h"
#include "HuskyParams.h"
#include <chrono>

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
            default:
                VSTGUI::VST3Editor::valueChanged(control);
                break;
            }
        }
    };
}}



