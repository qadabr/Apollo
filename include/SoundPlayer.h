#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <cstddef>

#include "SoundEngine.h"

#define PLAYER_SAMPLING_RATE 41000

class SoundPlayer
{
 public:
	SoundPlayer(SoundEngine* engine);
	~SoundPlayer();
 public:
	void Play();
	void Stop();
	void ClearQueue();
	void EnqueueBuffer(char* buffer, size_t bufferSize);
 private:
	char* m_buffer;
	size_t m_bufferSize;
		
	SoundEngine* m_engine;

	SLObjectItf m_outputMix;
	SLObjectItf m_player;

	SLPlayItf m_playerInterface;
	SLBufferQueueItf m_bufferQueueInterface;
};

#endif /* SOUNDPLAYER_H */
