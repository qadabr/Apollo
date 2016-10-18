#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "unistd.h"
#include <cmath>
#include <cstddef>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundPlayer
{
 public:
	SoundPlayer();
 public:
	void PlayTone(double freq, double duration);
 private:
	void createEngine();
	void getIidEngine();
	void createOutputMix();
	void createAudioPlayer();
	void getIidPlay();
	void getIidBufferQueue();
 private:
	SLObjectItf m_engineObj;
	SLEngineItf m_engine;
	SLObjectItf m_outputMixObj;
	SLObjectItf m_playerObj;
	SLPlayItf m_player;
	SLBufferQueueItf m_bufferQueue;
};

#endif /* SOUNDPLAYER_H */
