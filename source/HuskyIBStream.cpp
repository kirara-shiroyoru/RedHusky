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