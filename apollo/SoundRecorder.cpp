#include "SoundRecorder.h"

static void recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
	SoundRecorder* soundRecorder = (SoundRecorder *)context;
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

	result = (*m_recorder)->Realize(m_recorder, SL_BOOLEAN_FALSE);

	result = (*m_recorder)->GetInterface(m_recorder,
					     SL_IID_RECORD,
					     &m_recorderInterface);

	result = (*m_recorder)->GetInterface(m_recorder,
					     SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
					     &m_recorderQueueInterface);

	result = (*m_recorderQueueInterface)->RegisterCallback(
                   m_recorderQueueInterface, recorderCallback, this);
}

SoundRecorder::~SoundRecorder()
{
	
}
