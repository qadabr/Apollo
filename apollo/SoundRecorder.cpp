#include "SoundRecorder.h"

void SoundRecorder::recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	((SoundRecorder *)context)->Enqueue();
}

void SoundRecorder::Enqueue()
{
	LOG_D("Started to enqueue buffer...\n");
	uint16_t* buffer = new uint16_t[m_samplingRate * CHUNK_SIZE];

	m_lock.lock();
	(*m_recorderQueueInterface)->Enqueue
		(m_recorderQueueInterface,
		 buffer, m_samplingRate * CHUNK_SIZE * sizeof(uint16_t));
	m_queue.push(buffer);
	m_lock.unlock();

	for (size_t i = 0; i < m_samplingRate * CHUNK_SIZE; ++i) {
		LOG_I("buffer[%u] = %u\n", i, buffer[i]);
	}
}

SoundRecorder::SoundRecorder(SoundEngine* engine, uint32_t samplingRate)
	: m_engine(engine), m_samplingRate(samplingRate)
{
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
		(m_recorderQueueInterface, SoundRecorder::recorderCallback, this);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to register the recorder callback!\n");
		return;
	}

	//recorderCallback(NULL, this);
	
	//this->Stop();
}

SoundRecorder::~SoundRecorder()
{
	
}

SLRecordItf SoundRecorder::GetInterface()
{
	return m_recorderInterface;
}

void SoundRecorder::Record()
{
	SLresult result = (*m_recorderInterface)->SetRecordState(m_recorderInterface,
								 SL_RECORDSTATE_RECORDING);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to set record state to recording!\n");
		return;
	}
}

void SoundRecorder::Stop()
{
	SLresult result = (*m_recorderInterface)->SetRecordState(m_recorderInterface,
								 SL_RECORDSTATE_STOPPED);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to set record state to stopped!\n");
		return;
	}
	
	result = (*m_recorderQueueInterface)->Clear(m_recorderQueueInterface);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to clear the recorder queue!\n");
		return;
	}
}

uint32_t SoundRecorder::GetSamplingRate()
{
	return m_samplingRate;
}
