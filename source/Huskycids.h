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

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace shiroyoru {
//------------------------------------------------------------------------
static const Steinberg::FUID kRedHuskyProcessorUID (0xC403ADE2, 0xBC03549F, 0xB4EBD79D, 0x04B3B750);
static const Steinberg::FUID kRedHuskyControllerUID (0x02388F53, 0x457E5F55, 0x81DF67CA, 0xD12A87A4);

#define RedHuskyVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace shiroyoru
