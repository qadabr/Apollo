#ifndef SILVERPUSH_H
#define SILVERPUSH_H

#include "SoundEngine.h"
#include "SoundPlayer.h"
#include "SoundRecorder.h"

#include <string>
#include <cmath>

class SilverPush
{
 public:
	SilverPush(uint32_t samplingRate, double minFreq, double maxFreq, size_t duration);
	~SilverPush();
 public:
	void SendMessage(const std::string& message);
	std::string ReceiveMessage();
 private:
	char* generateWave(const std::string& message, size_t* bufferSize);
	double frameFrequency(int16_t* buffer, size_t pointA, size_t pointB);
 private:
	uint32_t m_samplingRate;
	double m_minFreq;
	double m_maxFreq;
	size_t m_duration;
	
	SoundEngine* m_engine;
	SoundPlayer* m_player;
	SoundRecorder* m_recorder;
};

#endif /* SILVERPUSH_H */
