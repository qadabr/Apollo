#ifndef SILVERPUSH_H
#define SILVERPUSH_H

#include "SoundEngine.h"
#include "SoundPlayer.h"
#include "SoundRecorder.h"
#include "Butterworth.h"

#include <vector>
#include <string>
#include <cmath>
#include <unistd.h>

class SilverPush
{
 public:
	SilverPush(SoundEngine* engine,
		   uint32_t samplingRate,
		   double minFreq,
		   double maxFreq,
		   size_t duration);
	~SilverPush();
 public:
	void PushMessage(const std::string& message);
	void Send();
	void ReceiveMessage();
 private:
	int16_t* generateWave(const std::string& message, size_t* bufferSize);
	double frequencyFilter(double freq);
	double frameFrequency(int16_t* buffer, size_t x0, size_t frameN);
 private:
	uint32_t m_samplingRate;
	double m_minFreq;
	double m_maxFreq;
	size_t m_duration;
	
	SoundEngine* m_engine;
	SoundPlayer* m_player;
	SoundRecorder* m_recorder;

	Butterworth* m_highpassFilter;
};

#endif /* SILVERPUSH_H */
