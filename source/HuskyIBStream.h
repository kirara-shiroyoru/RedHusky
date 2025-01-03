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

#include "pluginterfaces/base/funknownimpl.h"
#include "pluginterfaces/base/ibstream.h"
#include <memory>

namespace shiroyoru {
	namespace Husky {
		using namespace  Steinberg;
		class StreamInterface : public U::Implements< U::Directly<IBStream>> {
			std::unique_ptr<char[]> c_ptr;
			size_t size;
			size_t cursor = 0;

		public:
			StreamInterface(size_t size) : c_ptr(new char[size]), size(size) {}
			tresult PLUGIN_API read(void* buffer, int32 numBytes, int32* numBytesRead) SMTG_OVERRIDE;
			tresult PLUGIN_API write(void* buffer, int32 numBytes, int32* numBytesWritten) SMTG_OVERRIDE;
			tresult PLUGIN_API seek(int64 pos, int32 mode, int64* result) SMTG_OVERRIDE;
			tresult PLUGIN_API tell(int64* pos) SMTG_OVERRIDE {
				if (pos) {
					*pos = cursor;
					return kResultTrue;
				}
				return kInvalidArgument;
			}
			char* getPtr() const { return c_ptr.get(); }
			void wind() {
				cursor = 0;
			}
		};
	}
}