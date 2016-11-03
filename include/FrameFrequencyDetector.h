#ifndef FRAMEFREQUENCYDETECTOR_H
#define FRAMEFREQUENCYDETECTOR_H

#include <cstdint>
#include <vector>
#include <cmath>
#include <aquila/global.h>
#include <aquila/transform/FftFactory.h>

class FrameFrequencyDetector
{
 public:
	FrameFrequencyDetector(const std::vector<short>& frame);
	~FrameFrequencyDetector();

	const double GetMagnitude(const double frequency, const size_t samplingRate);	
	const double GetMaxMagnitude();
	
 private:
	size_t                   mSize;
	Aquila::SpectrumType     mSpectrum;
};

#endif /* FRAMEFREQUENCYDETECTOR_H */
