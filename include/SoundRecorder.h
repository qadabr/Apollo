#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

#include <cstdint>
#include <mutex>
#include <queue>

#include "SoundEngine.h"

// Размер куска, сохраняемого в буффер за коллбэк
#define PIECE_SIZE 5

class SoundRecorder
{
 public:
	SoundRecorder(SoundEngine* engine, uint32_t samplingRate);
	~SoundRecorder();

	SLRecordItf GetInterface();
	SLAndroidSimpleBufferQueueItf GetQueueInterface();

	void Record();
	void Stop();

	uint32_t GetSamplingRate();
 private:
	static void recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);
 private:
	SoundEngine* m_engine;
	uint32_t m_samplingRate;

	char* m_buffer;
	size_t m_bufferSize;
	
	SLObjectItf m_recorder;
	SLRecordItf m_recorderInterface;
	SLAndroidSimpleBufferQueueItf m_recorderQueueInterface;

	static std::mutex g_lock;
	static std::queue<uint16_t*> g_queue;
};

#endif /* SOUNDRECORDER_H */
