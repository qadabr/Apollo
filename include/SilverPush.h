#ifndef SILVERPUSH_H
#define SILVERPUSH_H

#include "SoundEngine.h"
#include "SoundPlayer.h"

#include <string>
#include <cmath>

#define SAMPLING_RATE 44100.0

class SilverPush
{
 public:
	SilverPush(double minFreq, double maxFreq, size_t duration);
	~SilverPush();
 public:
	void SendMessage(const std::string& message);
 private:
	char* generateWave(const std::string& message);
 private:
	double m_minFreq;
	double m_maxFreq;
	size_t m_duration;
	
	SoundEngine* m_engine;
	SoundPlayer* m_player;
};

#endif /* SILVERPUSH_H */
