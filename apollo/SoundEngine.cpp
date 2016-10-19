#include "SoundEngine.h"

bool SoundEngine::g_isCreated = false;

SoundEngine::SoundEngine()
{
	if (g_isCreated) {
		LOG_E("Sound engine already created!\n");
	}
	
	createEngine();
}

SoundEngine::~SoundEngine()
{
	
}

SLEngineItf SoundEngine::GetInterface() {
	return m_soundEngineInterface;
}

void SoundEngine::createEngine()
{
	const SLInterfaceID pIDs[] = { SL_IID_ENGINE };
	const SLboolean pIDsRequired[]  = { SL_BOOLEAN_TRUE };

	SLresult result = slCreateEngine(&m_soundEngine,
					 0, NULL,
					 1, pIDs,
					 pIDsRequired);
	
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to create the sound engine!\n");
		return;
	}
	
	result = (*m_soundEngine)->Realize(m_soundEngine, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to realize the sound engine!\n");
		return;
	}

	result = (*m_soundEngine)->GetInterface(m_soundEngine,
						SL_IID_ENGINE,
						&m_soundEngineInterface);
	
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to get sound engine interface!\n");
		return;
	}
	
	g_isCreated = true;
}
