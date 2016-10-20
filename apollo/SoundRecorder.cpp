#include "SoundRecorder.h"

std::mutex SoundRecorder::g_lock;
std::queue<uint16_t*> SoundRecorder::g_queue;

void SoundRecorder::recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	LOG_D("callback\n");
	SoundRecorder* recorder = (SoundRecorder *)context;

	uint16_t* buffer = new uint16_t[recorder->GetSamplingRate() * CHUNK_SIZE];
	SLAndroidSimpleBufferQueueItf queueItf = recorder->GetQueueInterface();

	g_lock.lock();
	(*queueItf)->Enqueue
		(queueItf, buffer, recorder->GetSamplingRate() * CHUNK_SIZE * sizeof(uint16_t));
	g_queue.push(buffer);
	g_lock.unlock();

	for (size_t i = 0; i < recorder->GetSamplingRate() * CHUNK_SIZE; ++i) {
		LOG_I("buffer[%u] = %u\n", i, buffer[i]);
	}
}

SoundRecorder::SoundRecorder(SoundEngine* engine, uint32_t samplingRate)
	: m_engine(engine), m_samplingRate(samplingRate)
{
	m_buffer = nullptr;
	m_bufferSize = 0;

	SLDataLocator_IODevice locatorDevice = {
		SL_DATALOCATOR_IODEVICE,
		SL_IODEVICE_AUDIOINPUT,
		SL_DEFAULTDEVICEID_AUDIOINPUT,
		NULL
	};
	
	SLDataSource audioSrc = {
		&locatorDevice,
		NULL
	};

	SLDataLocator_AndroidSimpleBufferQueue locatorBufferQueue = {
		SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
		2
	};

	SLDataFormat_PCM formatPCM = {
		SL_DATAFORMAT_PCM,
		1,
		m_samplingRate * 1000,
		SL_PCMSAMPLEFORMAT_FIXED_16,
		SL_PCMSAMPLEFORMAT_FIXED_16,
		SL_SPEAKER_FRONT_CENTER,
		SL_BYTEORDER_LITTLEENDIAN
	};

	SLDataSink audioSnk = {
		&locatorBufferQueue,
		&formatPCM
	};

	const SLInterfaceID pAudioRecorderIDs[] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
	const SLboolean pAudioRecorderRequired[] = { SL_BOOLEAN_TRUE };

	SLresult result
		= (*m_engine->GetInterface())->CreateAudioRecorder(m_engine->GetInterface(),
								   &m_recorder,
								   &audioSrc,
								   &audioSnk,
								   1,
								   pAudioRecorderIDs,
								   pAudioRecorderRequired);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to create the audio recorder!\n");
		return;
	}
	
	result = (*m_recorder)->Realize(m_recorder, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to realize the audio recorder!\n");
		return;
	}

	result = (*m_recorder)->GetInterface(m_recorder,
					     SL_IID_RECORD,
					     &m_recorderInterface);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed getting the recorder interface!\n");
		return;
	}
	
	result = (*m_recorder)->GetInterface(m_recorder,
					     SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
					     &m_recorderQueueInterface);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed getting the recorder queue interface!\n");
		return;
	}

	result = (*m_recorderQueueInterface)->RegisterCallback
		(m_recorderQueueInterface, recorderCallback, this);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to register recorder callback!\n");
		return;
	}
	
	this->Stop();
}

SoundRecorder::~SoundRecorder()
{
	
}

SLRecordItf SoundRecorder::GetInterface()
{
	return m_recorderInterface;
}

SLAndroidSimpleBufferQueueItf SoundRecorder::GetQueueInterface()
{
	return m_recorderQueueInterface;
}

void SoundRecorder::Record()
{
	SLresult result = (*m_recorderInterface)->SetRecordState(m_recorderInterface,
								 SL_RECORDSTATE_RECORDING);
}

void SoundRecorder::Stop()
{
	SLresult result = (*m_recorderInterface)->SetRecordState(m_recorderInterface,
								 SL_RECORDSTATE_STOPPED);

	result = (*m_recorderQueueInterface)->Clear(m_recorderQueueInterface); 
}

uint32_t SoundRecorder::GetSamplingRate()
{
	return m_samplingRate;
}
