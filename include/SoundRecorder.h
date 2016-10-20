#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <cstdint>
#include <mutex>
#include <queue>

#include "SoundEngine.h"

// Размер куска, сохраняемого в буффер за коллбэк
#define CHUNK_SIZE 10

class SoundRecorder
{
 public:
	SoundRecorder(SoundEngine* engine, uint32_t samplingRate);
	~SoundRecorder();

	SLRecordItf GetInterface();

	void Record();
	void Stop();
	void ClearBuffer();
	void Enqueue();
	char* Dequeue();

	uint32_t GetSamplingRate();
	size_t GetBufferSize();
 private:
	static void recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;

	size_t m_bufferSize;
	
	SLObjectItf m_recorder;
	SLRecordItf m_recorderInterface;
	SLAndroidSimpleBufferQueueItf m_recorderQueueInterface;

	std::mutex m_lock;
	std::queue<int16_t*> m_queue;
};

#endif /* SOUNDRECORDER_H */
