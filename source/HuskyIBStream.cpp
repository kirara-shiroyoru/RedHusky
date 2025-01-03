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
#include "HuskyIBStream.h"

namespace shiroyoru {
	namespace Husky {
		using namespace  Steinberg;
		tresult StreamInterface::read(void* buffer, int32 numBytes, int32* numBytesRead) {
			if (cursor >= size) {
				numBytes = 0;
				cursor = size;
			}
			else {
				if (cursor + numBytes > size) {
					numBytes = size - cursor;
				}
				memcpy(buffer, &c_ptr[cursor], static_cast<size_t> (numBytes));
				cursor += numBytes;
			}
			if (numBytesRead) *numBytesRead = numBytes;
			return kResultTrue;
		}
		tresult StreamInterface::write(void* buffer, int32 numBytes, int32* numBytesWritten) {
			if (cursor >= size) {
				numBytes = 0;
				cursor = size;
			}
			else {
				if (cursor + numBytes > size) {
					numBytes = size - cursor;
				}
				memcpy(&c_ptr[cursor], buffer, static_cast<size_t> (numBytes));
				cursor += numBytes;
			}
			if (numBytesWritten) *numBytesWritten = numBytes;
			return kResultTrue;
		}
		tresult StreamInterface::seek(int64 pos, int32 mode, int64* result) {
			switch (mode) {
			case kIBSeekSet:
				cursor = pos;
				break;
			case kIBSeekCur:
				cursor = cursor + pos;
				break;
			case kIBSeekEnd:
				cursor = size + pos;
				break;
			}
			if (cursor > size) cursor = size;
			if (result) *result = cursor;
			return kResultTrue;
		}
	}
}