#include "FrameFrequencyDetector.h"

FrameFrequencyDetector::FrameFrequencyDetector(const std::vector<short>& frame)
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

const double FrameFrequencyDetector::GetMagnitude(const double frequency, const size_t samplingRate)
{
	auto x = mSpectrum[mSize * frequency / samplingRate];
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
