#include "SoundRecorder.h"

void SoundRecorder::bufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void* context)
{
	LOG_I("Callback has been called!\n");
	((SoundRecorder *)context)->SaveChunk();
}

void SoundRecorder::SaveChunk()
{	
	size_t bufferSize = m_samplingRate * m_chunkSize * sizeof(int16_t);
	int16_t* buffer = new int16_t[bufferSize];

	(*m_recorderQueueInterface)->Enqueue(m_recorderQueueInterface,
					     buffer, bufferSize);

	m_lock.lock();
	m_queue.push(buffer);
	m_lock.unlock();
}

int16_t* SoundRecorder::DequeueBuffer()
{
	int16_t* buffer = nullptr;
	
	m_lock.lock();
	if (not m_queue.empty()) {
		buffer = m_queue.front();
		m_queue.pop();
	}
	m_lock.unlock();

	return buffer;
}

SoundRecorder::SoundRecorder(SoundEngine* engine, uint32_t samplingRate, uint32_t chunkSize)
	: m_engine(engine),
	  m_samplingRate(samplingRate),
	  m_chunkSize(chunkSize)
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
		SOUND_RECORDER_BUFFERS
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
		(m_recorderQueueInterface, SoundRecorder::bufferQueueCallback, this);

	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to register the recorder callback!\n");
		return;
	}
}

SoundRecorder::~SoundRecorder()
{
	this->Stop();
	(*m_recorder)->Destroy(m_recorder);
}

SLRecordItf SoundRecorder::GetInterface()
{
	return m_recorderInterface;
}

size_t SoundRecorder::GetBufferSize()
{
	return m_samplingRate * m_chunkSize;
}

useconds_t SoundRecorder::GetSwapTimeMicrosecond()
{
	return m_chunkSize * 10e6;
}

void SoundRecorder::Record()
{
	SLresult result = (*m_recorderInterface)->SetRecordState(m_recorderInterface,
								 SL_RECORDSTATE_RECORDING);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to set record state to recording!\n");
		return;
	}

	this->SaveChunk();
	delete[] this->DequeueBuffer();
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

void SoundRecorder::ClearQueue()
{
	SLresult result = (*m_recorderQueueInterface)->Clear(m_recorderQueueInterface);
	if (result != SL_RESULT_SUCCESS) {
		LOG_E("Failed to clear the recorder queue!\n");
		return;
	}

	LOG_D("The queue size is %u\n", m_queue.size());
	while (not m_queue.empty()) {
		int16_t* buffer = m_queue.front();
		delete[] buffer;
		m_queue.pop();
	}
	LOG_D("The queue is empty now!\n");
}

uint32_t SoundRecorder::GetSamplingRate()
{
	return m_samplingRate;
}
