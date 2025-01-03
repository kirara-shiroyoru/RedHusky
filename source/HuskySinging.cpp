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

#include <cmath>
#include "HuskySinging.h"

namespace shiroyoru {namespace Husky {
	struct FastSinNCalculator {
		constexpr static float fast_sin[] = { 0.0000000,0.0157073,0.0314108,0.0471065,0.0627905,0.0784591,
		0.0941083,0.1097343,0.1253332,0.1409012,0.1564345,0.1719291,0.1873813,0.2027873,
		0.2181433,0.2334454,0.2486899,0.2638731,0.2789911,0.2940403,0.3090170,0.3239174,
		0.3387379,0.3534749,0.3681246,0.3826834,0.3971479,0.4115144,0.4257793,0.4399392,
		0.4539905,0.4679298,0.4817537,0.4954587,0.5090414,0.5224986,0.5358268,0.5490229,
		0.5620834,0.5750053,0.5877853,0.6004202,0.6129071,0.6252427,0.6374240,0.6494481,
		0.6613119,0.6730126,0.6845472,0.6959128,0.7071068,0.7181263,0.7289687,0.7396311,
		0.7501111,0.7604060,0.7705133,0.7804304,0.7901551,0.7996847,0.8090170,0.8181497,
		0.8270806,0.8358074,0.8443280,0.8526402,0.8607421,0.8686315,0.8763067,0.8837657,
		0.8910066,0.8980276,0.9048271,0.9114033,0.9177547,0.9238795,0.9297765,0.9354440,
		0.9408808,0.9460854,0.9510565,0.9557930,0.9602937,0.9645574,0.9685832,0.9723699,
		0.9759168,0.9792228,0.9822873,0.9851093,0.9876884,0.9900237,0.9921147,0.9939610,
		0.9955620,0.9969173,0.9980267,0.9988899,0.9995066,0.9998766,1.0000000 };
		float sinX = 0.f, cosX = 0.f, sinNX = 0.f, cosNX = 1.f;
	public:
		FastSinNCalculator(float phase) {//0 to 400
			const int i = floor(phase), j = ceil(phase);
			const float x = phase - i;
			switch (i) {
			case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
			case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
			case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
			case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:
			case 40:case 41:case 42:case 43:case 44:case 45:case 46:case 47:case 48:case 49:
			case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:
			case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:
			case 70:case 71:case 72:case 73:case 74:case 75:case 76:case 77:case 78:case 79:
			case 80:case 81:case 82:case 83:case 84:case 85:case 86:case 87:case 88:case 89:
			case 90:case 91:case 92:case 93:case 94:case 95:case 96:case 97:case 98:case 99:
				sinX = wAVG(fast_sin[i], fast_sin[j], x);
				cosX = wAVG(fast_sin[100 - i], fast_sin[100 - j], x);
				break;
			case 100:case 101:case 102:case 103:case 104:case 105:case 106:case 107:case 108:case 109:
			case 110:case 111:case 112:case 113:case 114:case 115:case 116:case 117:case 118:case 119:
			case 120:case 121:case 122:case 123:case 124:case 125:case 126:case 127:case 128:case 129:
			case 130:case 131:case 132:case 133:case 134:case 135:case 136:case 137:case 138:case 139:
			case 140:case 141:case 142:case 143:case 144:case 145:case 146:case 147:case 148:case 149:
			case 150:case 151:case 152:case 153:case 154:case 155:case 156:case 157:case 158:case 159:
			case 160:case 161:case 162:case 163:case 164:case 165:case 166:case 167:case 168:case 169:
			case 170:case 171:case 172:case 173:case 174:case 175:case 176:case 177:case 178:case 179:
			case 180:case 181:case 182:case 183:case 184:case 185:case 186:case 187:case 188:case 189:
			case 190:case 191:case 192:case 193:case 194:case 195:case 196:case 197:case 198:case 199:
				sinX = wAVG(fast_sin[200 - i], fast_sin[200 - j], x);
				cosX = -wAVG(fast_sin[i - 100], fast_sin[j - 100], x);
				break;
			case 200:case 201:case 202:case 203:case 204:case 205:case 206:case 207:case 208:case 209:
			case 210:case 211:case 212:case 213:case 214:case 215:case 216:case 217:case 218:case 219:
			case 220:case 221:case 222:case 223:case 224:case 225:case 226:case 227:case 228:case 229:
			case 230:case 231:case 232:case 233:case 234:case 235:case 236:case 237:case 238:case 239:
			case 240:case 241:case 242:case 243:case 244:case 245:case 246:case 247:case 248:case 249:
			case 250:case 251:case 252:case 253:case 254:case 255:case 256:case 257:case 258:case 259:
			case 260:case 261:case 262:case 263:case 264:case 265:case 266:case 267:case 268:case 269:
			case 270:case 271:case 272:case 273:case 274:case 275:case 276:case 277:case 278:case 279:
			case 280:case 281:case 282:case 283:case 284:case 285:case 286:case 287:case 288:case 289:
			case 290:case 291:case 292:case 293:case 294:case 295:case 296:case 297:case 298:case 299:
				sinX = -wAVG(fast_sin[i - 200], fast_sin[j - 200], x);
				cosX = -wAVG(fast_sin[300 - i], fast_sin[300 - j], x);
				break;
			case 300:case 301:case 302:case 303:case 304:case 305:case 306:case 307:case 308:case 309:
			case 310:case 311:case 312:case 313:case 314:case 315:case 316:case 317:case 318:case 319:
			case 320:case 321:case 322:case 323:case 324:case 325:case 326:case 327:case 328:case 329:
			case 330:case 331:case 332:case 333:case 334:case 335:case 336:case 337:case 338:case 339:
			case 340:case 341:case 342:case 343:case 344:case 345:case 346:case 347:case 348:case 349:
			case 350:case 351:case 352:case 353:case 354:case 355:case 356:case 357:case 358:case 359:
			case 360:case 361:case 362:case 363:case 364:case 365:case 366:case 367:case 368:case 369:
			case 370:case 371:case 372:case 373:case 374:case 375:case 376:case 377:case 378:case 379:
			case 380:case 381:case 382:case 383:case 384:case 385:case 386:case 387:case 388:case 389:
			case 390:case 391:case 392:case 393:case 394:case 395:case 396:case 397:case 398:case 399:
				sinX = -wAVG(fast_sin[400 - i], fast_sin[400 - j], x);
				cosX = wAVG(fast_sin[i - 300], fast_sin[j - 300], x);
				break;
			}
		}
		/*
		FastSinNCalculator(float phase) {
			sinX = sin(phase);
			cosX = cos(phase);
		
		*/
		float nextSinN() {
			float tmp = sinNX;
			sinNX = sinNX * cosX + cosNX * sinX;
			cosNX = cosNX * cosX - tmp * sinX;
			return sinNX;
		}
	};
	//I decided to change my note model from linear ADSR to Linear A/R and exponential decay, so that it would be easier for me to fit
	class Note {
		constexpr static int a = 0;
		constexpr static int d = 1;
		//constexpr static int s = 3;
		constexpr static int r = 4;
		constexpr static int noMoreProcess = 0xFF;
		const int p_id;
		const float p_fqmtp;
		const float p_velocity;
		float p_phase = 0.f;
		float v1[numHarmonics] = { 0.f };
		float v2[numHarmonics] = {};
		unsigned char p_currentState[numHarmonics] = { 0 }; //attack = 0, decrease = 1, sustain = 2, release |=0x4 
		const SheetMusic& p_sheetMusic;
		Note(int id, float fqmtp, float velocity, SheetMusic& sheetMusic) :p_id(id), p_fqmtp(fqmtp), p_velocity(velocity), p_sheetMusic(sheetMusic) {
			for (auto& v : v2) { v = 1.f; }
		}
	public:
		static std::shared_ptr<Note> createNote(int id, float fqmtp, float velocity, SheetMusic& sheetMusic) {
			return std::make_shared<Note>(Note(id, fqmtp, velocity, sheetMusic));
		}
		int getId() const {
			return p_id;
		}
		void release() {
			for (auto& s : p_currentState) {
				s |= r;
			}
		}
		bool updateNote(float* out, int numSamples, const float updater[numHarmonics][5]) {
			//test code
			//TODO
			//why is this too slow? because i put the phase increment outside
			for (int i = 0; i < numSamples; ++i) {
				p_phase += p_sheetMusic.getDeltaMain() * p_fqmtp;
				//p_phase += 3.99;
				while (p_phase >= pi2) p_phase -= pi2;
				bool keepNote = false;
				auto fastSin = FastSinNCalculator(p_phase);
				for (int n = 0; n < numHarmonics; ++n) {
					const auto sinNX = fastSin.nextSinN();
					if (updater[n][vIndex] == 0.f)continue;
					switch (p_currentState[n]) {
					case a | r:
						v2[n] += updater[n][rIndex];
						if (v2[n] < minAcceptableV2) {
							p_currentState[n] = noMoreProcess;
							continue;
						}
						[[fallthrough]];
					case a:
						v1[n] += updater[n][aIndex];
						if (v1[n] > updater[n][vIndex]) {
							v1[n] = updater[n][vIndex] - updater[n][sIndex];
							p_currentState[n] |= d;
							out[i] += p_velocity * (v1[n] + updater[n][sIndex]) * v2[n] * sinNX;
						}
						else {
							out[i] += p_velocity * v1[n] * v2[n] * sinNX;
						}
						keepNote = true;
						continue;
						//should I use A d+s R model?
					case d | r:
						v2[n] += updater[n][rIndex];
						if (v2[n] < minAcceptableV2) {
							p_currentState[n] = noMoreProcess;
							continue;
						}
						[[fallthrough]];
					case d:
						v1[n] *= updater[n][dIndex];
						//v1[n] += updater[n][dIndex];
						//if (v1[n] < updater[n][sIndex]) {
						//	v1[n] = updater[n][sIndex];
						//	p_currentState[n] |= s;
						//}
						out[i] += p_velocity * (v1[n] + updater[n][sIndex]) * v2[n] * sinNX;
						keepNote = true;
						continue;
						//case s | r:
						//	v2[n] += updater[n][rIndex];
						//	if (v2[n] < minAcceptableV2) {
						//		p_currentState[n] = noMoreProcess;
						//		continue;
						//	}
						//	[[fallthrough]];
						//case s:
						//	keepNote = true;
						//	break;
					default:
						continue;
					}
				}
				
				if (!keepNote) return false;
			}
			return true;
		}
	};
	SheetMusic::SheetMusic() {
		setHarmonics(0, vIndex, 1.f);
		setHarmonics(0, aIndex, 0.001f);
		setHarmonics(0, dIndex, 0.01f);
		setHarmonics(0, sIndex, 0.5f);
		setHarmonics(0, rIndex, 0.001f);
		for (unsigned n = 1; n < numHarmonics; ++n) {
			setHarmonics(n, vIndex, 0.f);
			setHarmonics(n, aIndex, 0.001f);
			setHarmonics(n, dIndex, 0.01f);
			setHarmonics(n, sIndex, 0.5f);
			setHarmonics(n, rIndex, 0.001f);
		}
		for (unsigned k = 0; k < 12; ++k) {
			setKeyAdjust(k, powf(2.f, (float)k / 12) - 1.f);
		}
		setFqMain(norm440hz);
	}
	void SheetMusic::addNote(int id, int pitch, float velocity, float sampleRate) {
		//take pitch == 69 as main frequency
		const int mod_pitch = pitch + 3;
		const float fqmtp = (1 << (mod_pitch / 12)) * 0.015625f * p_keyAdjust[mod_pitch % 12];
		p_deltaTime = 1.f / sampleRate;
		if (id == -1) {
			if (p_id_creator == nullptr) {
				p_id_creator = std::make_unique<PitchIDCreator>(PitchIDCreator());
			}
			id = p_id_creator->getIDfromPitch(pitch);
		}
		p_notes.emplace_back(Note::createNote(id, fqmtp, velocity, *this));
	}
	void SheetMusic::releaseNote(int id, int pitch) {
		if (id == -1) {
			if (p_id_creator != nullptr) {
				id = p_id_creator->checkIDfromPitch(pitch);
			}
			if (id == -1) {
				p_notes.clear();
				return;
			}
		}
		for (auto& note : p_notes) {
			if (note->getId() == id) {
				note->release();
				return;
			}
		}
		//if no note is found, some error must have occured;
		p_notes.clear();
	}
	void SheetMusic::updateSamples(float** out, int numSamples, int numChannels) {
		if (numChannels <= 0) return;
		float updater[numHarmonics][5] = {};
		for (int n = 0; n < numHarmonics; ++n) {
			updater[n][vIndex] = getV1max(n);
			updater[n][sIndex] = getS(n);
			updater[n][aIndex] = getDeltaA(n);
			updater[n][dIndex] = getDeltaD(n);
			updater[n][rIndex] = getDeltaR(n);
		}
		//memset(out[0], 0, sizeof(float) * numSamples);//clear the output buffer of left
		for (int i = 0; i < numSamples; ++i) out[0][i] = 0.f;
		std::vector<std::shared_ptr<Note>> tmp_list;
		tmp_list.reserve(p_notes.size());
		for (auto& note : p_notes) {
			const auto r = note->updateNote(out[0], numSamples, updater);//is there anything else to update?
			if (r) tmp_list.emplace_back(note);//if yes, keep the note
		}
		p_notes = tmp_list;
		for(int chan = 1; chan<numChannels; ++chan)	memcpy(out[chan], out[0], numSamples * sizeof(float));
	}
}}