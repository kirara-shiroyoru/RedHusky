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

#include "HuskyLearning.h"
#include <cmath>
#include <vector>

namespace shiroyoru {namespace Husky {
	inline static float* handleArray(std::unique_ptr<float[]>& arr, unsigned f, unsigned n = 0) {
		return arr.get() + (size_t)f * numHarmonics + n;
		//return arr[(size_t)f * numHarmonics + n];
	}
	//Husky breakdown the note to learn
	//Husky just fit the vadsr relative to each harmonic
	constexpr ComplexNumber phi0(0.9999952938, -0.00306795676296);
	//husky is clever and does not woof while learning
	class HarmonicPointSet {
		float p_arr[numHarmonics];
	public:
		HarmonicPointSet() :p_arr{ 0 } {}
		HarmonicPointSet(float* points) {
			memcpy(p_arr, points, numHarmonics);
		}
		float& operator[](unsigned i) {
			return p_arr[i];
		}
		float operator[](unsigned i)const {
			return p_arr[i];
		}
	};
	void Computer::recognizeNoteDown(int pitch, float sampleRate) {
		if (!p_haveNote) {
			p_arr = std::vector<float>();
			p_releaseSampleIndex = 0;
			p_frequency = pow(2.f, (pitch - 69) / 12.f)*fqMain;
			p_sampleRate = sampleRate;
			p_haveNote = true;
		}
	}
	void Computer::recognizeNoteRelease() {
		if (!p_releaseSampleIndex) {
			p_releaseSampleIndex = p_arr.size();
			p_haveNote = false;
			p_hasResult = true;
		}
	}
	void Computer::listen(float** input, int numSamples) {
		for (int i = 0; i < numSamples; ++i) {
			//p_arr.push_back(0.f);
			p_arr.push_back(input[0][i]);
		}
	}
	bool Computer::conclude() {
		if (!p_hasResult) return false;
		//husky dft the samples for about every sampleRate/100 frame
		const unsigned step = p_sampleRate / 100;
		const unsigned numFrames = (p_arr.size() - dftLength) / step + 1;// how many frames can I create from this arr
		if (numFrames <= 0)return false;
		const unsigned releaseFrame = (p_releaseSampleIndex - dftLength) / step;//the frame that note is released
		if (releaseFrame <= 0)return false;
		if (releaseFrame > numFrames) return false;
		std::unique_ptr<float[]> data = std::unique_ptr<float[]>(new float[numFrames * numHarmonics]);
		int aFrame[numHarmonics] = { 0 };
		int rFrame[numHarmonics] = { 0 };
		float dValue[numHarmonics] = { 0 };
		float vMax[numHarmonics] = { 0 };
		float sVolume[numHarmonics] = { 0 };
		
		//above code no major error

				
		for (unsigned f = 0; f < numFrames; ++f) {
			fft(handleArray(data, f), f * step);
			for (unsigned n = 0; n < numHarmonics; ++n) {
				const float vol = *handleArray(data, f, n);
				if (vMax[n] <= vol) {
					aFrame[n] = f;//in frame index
					vMax[n] = vol;
				}
			}
		}
		p_arr.clear();

		for (int n = 0; n < numHarmonics; ++n) {
			sVolume[n] = *handleArray(data, releaseFrame - 1, n);
			//get all d data
			float sum_x = 0.f, sum_y = 0.f, sum_xx = 0.f, sum_xy = 0.f;
			const float delta = vMax[n] - sVolume[n];
			if (aFrame[n] < releaseFrame) {
				for (unsigned f = aFrame[n]; f < releaseFrame; ++f) {
					//model should be something f(x) = (vMax[n]-sustainV[n])*exp(d * X)+sustainV[n]
					//move, take log scale and fit d
					const float x = f; // unit frame
					const float y = log((*handleArray(data, f, n) - sVolume[n]) / delta);
					if (std::isfinite(y)) {
						sum_x += x;
						sum_y += y;
						sum_xx += x * x;
						sum_xy += x * y;
					}
				}
				//fit to a negative slope straight line
				const float ns = releaseFrame - aFrame[n];
				const auto m = (ns * sum_xy - sum_x * sum_y) / (ns * sum_xx - sum_x * sum_x);//unit 1/frame
				dValue[n] = -1.f / (m + epsilon); // unit frame
			}
			
			//get rData
			for (unsigned f = numFrames - 1; f >= releaseFrame; --f) {
				if (*handleArray(data,f,n) > minAcceptableV2) {
					rFrame[n] = f - releaseFrame;
					break;
				}
			}
		}

		float vTotalMax = 0.f;
		for (unsigned n = 0; n < numHarmonics; ++n) {
			vTotalMax = std::max(vTotalMax, vMax[n]);
		}
		const float sPerFrame = (float)step / p_sampleRate;
		for (int n = 0; n < numHarmonics; ++n) {
			p_harmonics[n][vIndex] = vMax[n] / vTotalMax;
			p_harmonics[n][sIndex] = sVolume[n] / vMax[n];
			p_harmonics[n][aIndex] = aFrame[n] * sPerFrame / maxTimeSeconds;
			p_harmonics[n][dIndex] = dValue[n] * sPerFrame / maxTimeSeconds;
			p_harmonics[n][rIndex] = rFrame[n] * sPerFrame / maxTimeSeconds;
		}
		p_releaseSampleIndex = 0;
		p_hasResult = false;
		return true;
	}
	void Computer::fft(float* set, unsigned startIndex) {
		//From https://rosettacode.org/wiki/Fast_Fourier_transform
		//std::vector<ComplexNumber> x;
		//x.reserve(dftLength);
		//for (int i = 0; i < dftLength; ++i) {	x.emplace_back(p_arr[i + startIndex]);}
		ComplexNumber x[dftLength];
		for (int i = 0; i < halfDftLength; ++i) {
			const int antiI = dftLength - 1 - i;
			x[i] = p_arr[(size_t)i + startIndex] * window[i];
			x[antiI] = p_arr[(size_t)antiI + startIndex] * window[i];
		}
		unsigned int N = dftLength, k = N, n;
		ComplexNumber phiT(phi0);
		while (k > 1)
		{
			n = k;
			k >>= 1;
			phiT = phiT * phiT;
			ComplexNumber T(1.0);
			for (unsigned int l = 0; l < k; l++)
			{
				for (unsigned int a = l; a < N; a += n)
				{
					unsigned int b = a + k;
					ComplexNumber t = x[a] - x[b];
					x[a] += x[b];
					x[b] = t * T;
				}
				T *= phiT;
			}
		}
		// Decimate
		for (unsigned int a = 0; a < N; a++)
		{
			unsigned int b = a;
			// Reverse bits in the length of array
			b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
			b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
			b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
			b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
			b = ((b >> 16) | (b << 16)) >> (32 - log2DftLength);
			if (b > a) //exchange value if b>a
			{
				ComplexNumber t = x[a];
				x[a] = x[b];
				x[b] = t;
			}
		}
		const auto iBase = p_frequency * dftLength / p_sampleRate;
		for (unsigned n = 0; n < numHarmonics; ++n) {
			const unsigned index = iBase * (n+1);
			if (index <= halfDftLength) {
				//set[n] = n;
				set[n] = std::sqrt(x[index].real * x[index].real + x[index].imag * x[index].imag);
			}
			else {
				set[n] = 0.f;
			}
		}
	}
}}

/*
std::vector<HarmonicPointSet> pointSets(numFrames);
HarmonicPointSet d, v1max, sustainV, r;
unsigned a[numHarmonics] = {0};
for (unsigned i = 0; i < numFrames; ++i) {
	fft(pointSets[i], i * step);//save volumes of that point in a pointSet
	for (unsigned n = 0; n < numHarmonics; ++n) {
		if (v1max[n] <= pointSets[i][n]) {
			a[n] = i;//in frame index
			v1max[n] = pointSets[i][n];
		}
	}
}
for (unsigned n = 0; n < numHarmonics; ++n) {
	sustainV[n] = pointSets[releaseFrame][n];
}
//after getting all the points, do the fitting
for (unsigned n = 0; n < numHarmonics; ++n) {
	std::vector<float> model;
	//get all d data
	float sum_x = 0.f, sum_y = 0.f, sum_xx = 0.f, sum_xy = 0.f;
	for (unsigned f = a[n]; f < releaseFrame; ++f) {
		//model should be something (v1max[n]-sustainV[n])*exp(dx)+sustainV[n]
		// eliminate sustainV component from data and take log scale
		// fit to log(v1max[n]-sustainV[n])+d(f-a[n])
		// so also remove log(v1max[n]-sustainV[n]) component
		float x = a[n]; // unit frame
		float y = log(pointSets[f][n] - sustainV[n]) - log(v1max[n] - sustainV[n]);
		sum_x += x;
		sum_y += y;
		sum_xx += x * x;
		sum_xy += x * y;
		//calculate the mse with differenct model
		//float recentVal = 0.f;
		//for (unsigned r = a[n]; r <= f; ++r) {
		//	const float diff = pointSets[r][n] - wAVG(v1max[n], sustainV[n], (r - a[n]) / (f - a[n]));
		//	recentVal += diff * diff;
		//}
		//const float lastSampleDiff = pointSets[f][n] - sustainV[n];
		//if (prevVal + lastSampleDiff * lastSampleDiff < recentVal) {//got local minimum
		//	d[n] = f - a[n];
		//	break;
		//}
		//else {
		//	prevVal = recentVal;
		//}
	}
	//fit to a negative slope straight line
	auto ns = releaseFrame - a[n];
	auto m = (ns * sum_xy - sum_x * sum_y) / (ns * sum_xx - sum_x * sum_x);//unit 1/frame
	d[n] = -1.f / m; // unit frame
	//get rData
	for (unsigned f = numFrames - 1; f >= releaseFrame; --f) {
		if (pointSets[f][n] > minAcceptableV2) {
			r[n] = f - releaseFrame;
			break;
		}
	}
}



		//now normalize all values
		//find the largest v1max among all
		float vmax = 0.f;
		for (int n = 0; n < numHarmonics; ++n) {
			if (v1max[n] > vmax)vmax = v1max[n];
		}
		const float sPerFrame = step / p_sampleRate;
		for (int n = 0; n < numHarmonics; ++n) {
			p_harmonics[n][vIndex] = v1max[n] / vmax;
			p_harmonics[n][sIndex] = sustainV[n] / v1max[n];
			p_harmonics[n][aIndex] = a[n] * sPerFrame / maxTimeSeconds;
			p_harmonics[n][dIndex] = d[n] * sPerFrame / maxTimeSeconds;
			p_harmonics[n][rIndex] = r[n] * sPerFrame / maxTimeSeconds;
		}
*/