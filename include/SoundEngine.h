#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "Logger.h"

#include <cstddef>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundEngine
{
 public:
	SoundEngine();
	~SoundEngine();

	SLEngineItf GetInterface();
 private:
	void createEngine();
 private:
	static bool g_isCreated;
	SLObjectItf m_soundEngine;
	SLEngineItf m_soundEngineInterface;
};

#endif /* SOUNDENGINE_H */
