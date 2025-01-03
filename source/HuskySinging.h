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
#include <memory>
#include "HuskyParams.h"
#include <vector>
#include <map>

namespace shiroyoru {namespace Husky {
	class Note;
	class PitchIDCreator {
		int p_accu = 0;
		std::map<int, int> p_idMap;
	public:
		int getIDfromPitch(int pitch) {
			p_idMap[p_accu] = pitch;
			return p_accu++;
		}
		int checkIDfromPitch(int pitch) {
			for (auto& it : p_idMap) {
				if (it.second == pitch) {
					auto r = it.first;
					p_idMap.erase(it.first);
					return r;
				}
			}
			return -1;//error happen, delete all notes
		}
	};
	class SheetMusic {
		std::vector<std::shared_ptr<Note>> p_notes;
		std::unique_ptr<PitchIDCreator> p_id_creator;
		float p_harmonics[numHarmonics][5] = {{0}};
		float p_keyAdjust[12] = {0};//decide how other 11 keys are adjusted
		float p_wMain = 176000.f; //angular velocity of the main frequency from 0 to 400
		float p_deltaTime = 2.26757396E-5f;//s per sample
		float getDeltaMain() const {
			return p_wMain * p_deltaTime;
		}
		float getV1max(int nthHarmonic) const {
			return p_harmonics[nthHarmonic][vIndex];
		}
		float getS(int nthHarmonic) const {
			return getV1max(nthHarmonic) * p_harmonics[nthHarmonic][sIndex];
		}
		float getDeltaA(int nthHarmonic) const {
			return getV1max(nthHarmonic) * p_deltaTime * p_harmonics[nthHarmonic][aIndex];
		}
		float getDeltaD(int nthHarmonic) const {
			return exp(-1.f * p_deltaTime * p_harmonics[nthHarmonic][dIndex]);
			//return (getS(nthHarmonic) - getV1max(nthHarmonic)) * p_deltaTime * p_harmonics[nthHarmonic][dIndex];
		}
		float getDeltaR(int nthHarmonic) const {
			return -1.f * p_deltaTime * p_harmonics[nthHarmonic][rIndex];
		}
		friend Note;
	public:
		SheetMusic();
		void setHarmonics(int nthHarmonic, unsigned typeIndex, float value) {
			switch (typeIndex) {
			case vIndex:
				p_harmonics[nthHarmonic][typeIndex] = maxAcceptableV1 * value;
				break;
			case sIndex:
				p_harmonics[nthHarmonic][typeIndex] = value;
				break;
			default:
				p_harmonics[nthHarmonic][typeIndex] = 1.f / maxTimeSeconds / (value + epsilon); // 1/(maxTimeSeconds/value);
				break;
			}
		}
		float getHarmonics(int nthHarmonic, unsigned typeIndex) const {
			switch (typeIndex) {
			case vIndex:
				return 1.f / maxAcceptableV1 * p_harmonics[nthHarmonic][typeIndex];
			case sIndex:
				return p_harmonics[nthHarmonic][typeIndex];
			default:
				return 1.f / maxTimeSeconds / p_harmonics[nthHarmonic][typeIndex] - epsilon;
			}
		}
		void setKeyAdjust(int key, float value) {
			p_keyAdjust[key] = 1.f + value;
		}
		float getKeyAdjust(int key) const {
			return p_keyAdjust[key] - 1.f;
		}
		void setFqMain(float value) {//from 0 to 1
			p_wMain = ((maxFq - minFq) * value + minFq) * pi2;
		}
		float getFqMain() const {
			return (p_wMain / pi2 - minFq) / (maxFq - minFq);
		}
		void addNote(int id, int pitch, float velocity, float sampleRate);
		void releaseNote(int id, int pitch);
		void updateSamples(float** out, int numSamples, int numChannels);
		void clearAllNotes() { p_notes.clear(); }
	};
}}
