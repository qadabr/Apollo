#ifndef SILVERRAY_H
#define SILVERRAY_H

#include "SoundEngine.h"
#include "SoundPlayer.h"
#include "SoundRecorder.h"

#include <aquila/global.h>
#include <aquila/transform/FftFactory.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <list>
#include <thread>

#include <unistd.h>

class SilverRay
{
 public:
	SilverRay(SoundEngine* engine,
		   uint32_t samplingRate,
		   double minFreq,
		   double maxFreq,
		   size_t duration);
	~SilverRay();
 public:
	void PushMessage(const std::string& message);
	void Send();
	void ReceiveMessage(size_t milliseconds);
 private:
	std::string GetBitList(std::list<double>& frequencySequence, size_t frameStep);
	void ParseBuffer(std::list<double>& frequencySequence,
			 int16_t* buffer,
			 size_t bufferSize,
			 size_t frameSize,
			 size_t frameStep);

	void FilterFrequencySequence(std::list<double>& frequencySequence);
	
	int16_t* GenerateWave(const std::string& message, size_t* bufferSize);
	void SignalFiltration(int16_t* buffer, size_t bufferSize, double filterFreq);
	double FrameFrequency(int16_t* buffer, size_t x0, size_t frameN);
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
