#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <cstddef>
#include <cstdint>

#include "SoundEngine.h"

class SoundPlayer
{
 public:
	SoundPlayer(SoundEngine* engine, uint32_t samplingRate);
	~SoundPlayer();
 public:
	void Play();
	void Stop();
	void ClearQueue();
	void EnqueueBuffer(char* buffer, size_t bufferSize);
	uint32_t GetSamplingRate();	
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;
	
	char* m_buffer;
	size_t m_bufferSize;

	SLObjectItf m_outputMix;
	SLObjectItf m_player;
	SLPlayItf m_playerInterface;
	SLBufferQueueItf m_bufferQueueInterface;
};

#endif /* SOUNDPLAYER_H */
