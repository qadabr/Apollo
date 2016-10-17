#ifndef SONICRAY_H
#define SONICRAY_H

#include "Logger.h"

#include <cstdint>
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SonicRay
{
 public:
	SonicRay(uint16_t sampleRate, double minFreq, double maxFreq);
	~SonicRay();
 public:
	void Send(const std::string& message);
 private:
	uint16_t m_sampleRate;
	double m_minFreq;
	double m_maxFreq;

	SLObjectItf m_slObject;
	SLEngineItf m_slEngine;
};

#endif /* SONICRAY_H */
