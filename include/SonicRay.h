#ifndef SONICRAY_H
#define SONICRAY_H

#include "Logger.h"

#include <cstdint>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SonicRay
{
 public:
	SonicRay(uint32_t sampleRate, double minFreq, double maxFreq);
	~SonicRay();
 private:
	uint32_t m_sampleRate;
	double m_minFreq;
	double m_maxFreq;
	SLObjectItf m_slInterface;
	SLresult m_slEngine;
};

#endif /* SONICRAY_H */
