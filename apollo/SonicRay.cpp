#include "SonicRay.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

SonicRay::SonicRay(uint16_t sampleRate, double minFreq, double maxFreq)
	: m_sampleRate(sampleRate), m_minFreq(minFreq), m_maxFreq(maxFreq)
{
	const SLInterfaceID pIDs[1] = { SL_IID_ENGINE };
	const SLboolean pIDsRequired[1] = { SL_BOOLEAN_TRUE };

	SLresult result = slCreateEngine(&m_slObject,
				    0, NULL,
				    1, pIDs,
				    pIDsRequired);
	
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to do slCreateEngine!\n");
		return;
	}

	result = (*m_slObject)->Realize(m_slObject, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed after realize slObject!\n");
		return;
	}

	LOG_D("SonicRay has been created successfully!\n");
}

SonicRay::~SonicRay()
{
	
}

void SonicRay::Send(const std::string& message)
{
	
}
