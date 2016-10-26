#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <cstdint>
#include <mutex>
#include <queue>

#include "SoundEngine.h"
#include "Setting.h"

class SoundRecorder
{
 public:
	SoundRecorder(SoundEngine* engine, uint32_t samplingRate, uint32_t chunkSize);
	~SoundRecorder();

	SLRecordItf GetInterface();

	void Record();
	void Stop();
	void ClearQueue();
	void SaveChunk();
	int16_t* DequeueBuffer();

	uint32_t GetSamplingRate();
	size_t GetBufferSize();

	useconds_t GetSwapTimeMicrosecond();
 private:
	static void bufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;
	uint32_t m_chunkSize;

	SLObjectItf m_recorder;
	SLRecordItf m_recorderInterface;
	SLAndroidSimpleBufferQueueItf m_recorderQueueInterface;

	std::mutex m_lock;
	std::queue<int16_t*> m_queue;
};

#endif /* SOUNDRECORDER_H */
