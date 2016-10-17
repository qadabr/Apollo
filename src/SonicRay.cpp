#include "SonicRay.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

SonicRay::SonicRay(uint32_t sampleRate, double minFreq, double maxFreq)
	: m_sampleRate(sampleRate), m_minFreq(minFreq), m_maxFreq(maxFreq)
{
	const SLInterfaceID pIDs[1] = { SL_IID_ENGINE };
}
