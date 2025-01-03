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

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace shiroyoru {
//------------------------------------------------------------------------
static const Steinberg::FUID kRedHuskyProcessorUID (0xC403ADE2, 0xBC03549F, 0xB4EBD79D, 0x04B3B750);
static const Steinberg::FUID kRedHuskyControllerUID (0x02388F53, 0x457E5F55, 0x81DF67CA, 0xD12A87A4);

#define RedHuskyVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace shiroyoru
