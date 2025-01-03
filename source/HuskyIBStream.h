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