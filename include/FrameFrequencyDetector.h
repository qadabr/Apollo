#ifndef FRAMEFREQUENCYDETECTOR_H
#define FRAMEFREQUENCYDETECTOR_H

#include <cstdint>
#include <vector>
#include <cmath>
#include <algorithm>
#include <aquila/global.h>
#include <aquila/transform/FftFactory.h>

class FrameFrequencyDetector
{
 public:
	FrameFrequencyDetector(const std::vector<short>& frame);
	~FrameFrequencyDetector();

	void HighFilter(const double frequency, const size_t samplingRate);
	
	const double GetMagnitude(const double frequency, const size_t samplingRate);	
	const double GetMaxMagnitude();
	
 private:
	size_t                   mSize;
	Aquila::SpectrumType     mSpectrum;
};

#endif /* FRAMEFREQUENCYDETECTOR_H */
