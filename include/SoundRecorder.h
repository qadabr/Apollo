#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <cstdint>

#include "SoundEngine.h"

class SoundRecorder
{
 public:
	SoundRecorder(SoundEngine* engine, uint32_t samplingRate);
	~SoundRecorder();

	void Listen();
	void Stop();

	SLRecordItf GetInterface();
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;

	char* m_buffer;
	size_t m_bufferSize;
	
	SLObjectItf m_recorder;
	SLRecordItf m_recorderInterface;
	SLAndroidSimpleBufferQueueItf m_recorderQueueInterface;
};

#endif /* SOUNDRECORDER_H */
