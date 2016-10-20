#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <cstdint>
#include <mutex>
#include <queue>

#include "SoundEngine.h"

// Размер куска, сохраняемого в буффер за коллбэк
#define CHUNK_SIZE 5

class SoundRecorder
{
 public:
	SoundRecorder(SoundEngine* engine, uint32_t samplingRate);
	~SoundRecorder();

	SLRecordItf GetInterface();

	void Record();
	void Stop();
	void Enqueue();

	uint32_t GetSamplingRate();
 private:
	static void recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;
	
	SLObjectItf m_recorder;
	SLRecordItf m_recorderInterface;
	SLAndroidSimpleBufferQueueItf m_recorderQueueInterface;

	std::mutex m_lock;
	std::queue<uint16_t*> m_queue;
};

#endif /* SOUNDRECORDER_H */
