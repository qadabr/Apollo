#include "FrameFrequencyDetector.h"

FrameFrequencyDetector::FrameFrequencyDetector(const std::vector<short>& frame, const size_t samplingRate)
	: mSamplingRate(samplingRate)
{
	// Для быстрого проеобразования фурье нам нужен массив размером степени двойки
	mSize = 2;
	while (mSize < frame.size()) {
		mSize *= 2;
	}

	// Выедяем память под фрейм с новым размером и копируем туда frame
	std::vector<double> newFrame(mSize, 0);
	std::copy(frame.begin(), frame.end(), newFrame.begin());

	// Спектр, получившийся после преобразования Фурье
	mSpectrum = Aquila::FftFactory::getFft(mSize)->fft(newFrame.data());
}

FrameFrequencyDetector::~FrameFrequencyDetector()
{
	
}

void FrameFrequencyDetector::HighFilter(const double frequency)
{
	Aquila::SpectrumType filterSpectrum(mSize);
	for (std::size_t i = 0; i < mSize; ++i) {
		double freq1 = mSize * frequency / mSamplingRate;
		double freq2 = mSize * (mSamplingRate - frequency) / mSamplingRate; 
		
		if (i < freq1 || i > freq2) {
			filterSpectrum[i] = 0.0;
		}
		else {
			filterSpectrum[i] = 2.0;
		}
	}

	std::transform(mSpectrum.begin(),
		       mSpectrum.end(),
		       filterSpectrum.begin(),
		       mSpectrum.begin(),
		       [] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y; });
}

const double FrameFrequencyDetector::GetMagnitude(const double frequency)
{
	auto x = mSpectrum[mSize * frequency / mSamplingRate];
	return std::sqrt(x.real() * x.real() + x.imag() * x.imag());
}

const double FrameFrequencyDetector::GetMaxMagnitude()
{
	double maximum = 0;
	for (auto x : mSpectrum) {
		double current = std::sqrt(x.real() * x.real() + x.imag() * x.imag());
		if (current > maximum) {
			maximum = current;
		}
	}

	return maximum;
}
