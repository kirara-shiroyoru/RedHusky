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
#include "HuskyParams.h"
#include<memory>
#include<vector>

namespace shiroyoru {namespace Husky {
	struct ComplexNumber {
		float real, imag;
		constexpr ComplexNumber(float real = 0.0, float imag = 0.f) :real(real), imag(imag) {};
		constexpr ComplexNumber operator+(const ComplexNumber& c) const { return ComplexNumber(real + c.real, imag + c.imag); }
		constexpr ComplexNumber operator+(float f) const { return ComplexNumber(real + f, imag); }
		constexpr ComplexNumber operator-(const ComplexNumber& c) const { return ComplexNumber(real - c.real, imag - c.imag); }
		constexpr ComplexNumber operator-(float f) const { return ComplexNumber(real + f, imag); }
		constexpr ComplexNumber operator*(const ComplexNumber& c) const {
			return ComplexNumber(real * c.real - imag * c.imag, real * c.imag + imag * c.real);
		}
		constexpr ComplexNumber operator*(float f) const { return ComplexNumber(real * f, imag * f); }
		constexpr ComplexNumber operator/(const ComplexNumber& c) const {
			const float divide = 1.f / ((c.real) * (c.real) + (c.imag) * (c.imag));
			return ComplexNumber((real * c.real + imag * c.imag) * divide, (imag * c.real - real * c.imag) * divide);
		}
		constexpr ComplexNumber operator/(float f) const { return ComplexNumber(real * f, imag * f); }
		constexpr ComplexNumber& operator = (const ComplexNumber& c) {
			real = c.real;
			imag = c.imag;
			return *this;
		}
		constexpr ComplexNumber& operator = (float f) {
			real = f;
			imag = 0.f;
			return *this;
		}
		constexpr ComplexNumber& operator += (const ComplexNumber& c) { return (*this = *this + c); }
		constexpr ComplexNumber& operator += (float f) { return (*this = *this + f); }
		constexpr ComplexNumber& operator -= (const ComplexNumber& c) { return (*this = *this - c); }
		constexpr ComplexNumber& operator -= (float f) { return (*this = *this - f); }
		constexpr ComplexNumber& operator *= (const ComplexNumber& c) { return (*this = *this * c); }
		constexpr ComplexNumber& operator *= (float f) { return (*this = *this * f); }
		constexpr ComplexNumber& operator /= (const ComplexNumber& c) { return (*this = *this / c); }
		constexpr ComplexNumber& operator /= (float f) { return (*this = *this / f); }
		constexpr operator float() const { return real; }
		constexpr void applyConj() { imag = -imag; }
	};
	class ComplexArray {
		std::vector<ComplexNumber> p_array;
	public:
		ComplexArray() {}
		ComplexArray(unsigned int length) {
			p_array.reserve(length);
		}
		ComplexArray(float* values, unsigned int length) {
			p_array.reserve(length);
			for (unsigned i = 0; i < length; ++i) {
				p_array.emplace_back(values[i]);
			}
		}
		ComplexArray(const ComplexArray& cArray) {
			p_array = std::vector(cArray.p_array);
		}
		ComplexArray& operator = (const ComplexArray& cArr) {
			p_array = std::vector(cArr.p_array);
			return *this;
		}
		ComplexArray& operator += (const ComplexArray& cArr) {
			auto length = std::min(p_array.size(), cArr.p_array.size());
			for (unsigned i = 0; i < length; ++i) {
				p_array[i] += cArr.p_array[i];
			}
			return *this;
		}
		ComplexArray& operator -= (const ComplexArray& cArr) {
			auto length = std::min(p_array.size(), cArr.p_array.size());
			for (unsigned i = 0; i < length; ++i) {
				p_array[i] -= cArr.p_array[i];
			}
			return *this;
		}
		ComplexArray& operator *= (const ComplexArray& cArr) {
			auto length = std::min(p_array.size(), cArr.p_array.size());
			for (unsigned i = 0; i < length; ++i) {
				p_array[i] *= cArr.p_array[i];
			}
			return *this;
		}
		ComplexArray& operator /= (const ComplexArray& cArr) {
			auto length = std::min(p_array.size(), cArr.p_array.size());
			for (unsigned i = 0; i < length; ++i) {
				p_array[i] /= cArr.p_array[i];
			}
			return *this;
		}
		void applyConj() {
			for (unsigned int i = 0; i < p_array.size(); ++i) {
				p_array[i].applyConj();
			}
		}
		void reserveMore(unsigned i) {
			p_array.reserve(p_array.size() + i);
		}
		void emplace_back(float f) {
			p_array.emplace_back(f);
		}
		void emplace_back(const ComplexNumber& c) {
			p_array.emplace_back(c);
		}
		ComplexNumber& operator [](unsigned i) {
			return p_array[i];
		}
		ComplexNumber operator [](unsigned i) const {
			return p_array[i];
		}
		size_t size()const {
			return p_array.size();
		}
	};
	class HarmonicPointSet;
	class Computer {
		float fqMain = 440.f;
		float p_sampleRate = 44100.f;
		float p_frequency = 440.f;
		bool p_haveNote = false;
		unsigned p_releaseSampleIndex = 0;
		std::vector<float> p_arr;
		float p_harmonics[numHarmonics][5] = {{0}};
		bool p_hasResult = false;
		void fft(float* arr, unsigned startIndex);
	public:
		void setFrequency(float value) { fqMain = (maxFq - minFq) * value + minFq; }
		void recognizeNoteDown(int pitch, float sampleRate);
		void recognizeNoteRelease();
		void listen(float** input, int numSamples);
		bool conclude();
		void clearAllTask() {
			p_arr = std::vector<float>();
		}
		float getResult(int nthHarmonic, int typeIndex) const {
			return p_harmonics[nthHarmonic][typeIndex];
		}
		bool hasResult() const { return p_hasResult; }
	};
}}