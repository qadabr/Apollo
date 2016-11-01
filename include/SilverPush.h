#ifndef SILVERPUSH_H
#define SILVERPUSH_H

#include "SoundEngine.h"
#include "SoundPlayer.h"
#include "SoundRecorder.h"
#include "Butterworth.h"

#include <aquila/global.h>
#include <aquila/transform/FftFactory.h>
#include <aquila/tools/TextPlot.h>
#include <aquila/source/generator/SineGenerator.h>
#include <algorithm>
#include <functional>
#include <memory>

#include <vector>
#include <string>
#include <cmath>
#include <list>
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
	std::string GetBitList(std::list<double>& frequencySequence, size_t frameStep);
	void ParseBuffer(std::list<double>& frequencySequence,
			 int16_t* buffer,
			 size_t bufferSize,
			 size_t frameSize,
			 size_t frameStep);

	void GetFrequency(int16_t buffer[], size_t startIndex, size_t frameSize, double& minA, double& maxA);
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
