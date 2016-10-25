#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(SoundEngine* engine, uint32_t samplingRate)
	: m_engine(engine), m_samplingRate(samplingRate)
{
	m_buffer = nullptr;
	m_bufferSize = 0;
	
	const SLInterfaceID pOutputMixIDs[] = {};
	const SLboolean pOutputMixRequired[] = {};

	SLresult result = (*m_engine->GetInterface())->CreateOutputMix(
						       m_engine->GetInterface(),
						       &m_outputMix,
						       0, pOutputMixIDs,
						       pOutputMixRequired);
	
	result = (*m_outputMix)->Realize(m_outputMix, SL_BOOLEAN_FALSE);

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
	
	SLDataSource audioSrc = {
		&locatorBufferQueue,
		&formatPCM
	};
	
	SLDataLocator_OutputMix locatorOutMix = {
		SL_DATALOCATOR_OUTPUTMIX,
		m_outputMix
	};
	
	SLDataSink audioSnk = { &locatorOutMix, NULL };
	const SLInterfaceID pIDs[] = { SL_IID_BUFFERQUEUE };
	const SLboolean pIDsRequired[] = { SL_BOOLEAN_TRUE };

	result = (*m_engine->GetInterface())->CreateAudioPlayer(m_engine->GetInterface(),
								&m_player,
								&audioSrc,
								&audioSnk,
								1, pIDs,
								pIDsRequired);
	
	result = (*m_player)->Realize(m_player, SL_BOOLEAN_FALSE);

	result = (*m_player)->GetInterface(m_player,
					   SL_IID_PLAY,
					   &m_playerInterface);

	result = (*m_player)->GetInterface(m_player,
					   SL_IID_BUFFERQUEUE,
					   &m_bufferQueueInterface);
	
	this->Stop();
}


SoundPlayer::~SoundPlayer()
{
	
}

void SoundPlayer::Play()
{
	SLresult result = (*m_playerInterface)->SetPlayState(m_playerInterface,
							     SL_PLAYSTATE_PLAYING);
}

void SoundPlayer::Stop()
{
	SLresult result = (*m_playerInterface)->SetPlayState(m_playerInterface,
							     SL_PLAYSTATE_STOPPED);
}

void SoundPlayer::ClearQueue()
{
	if (m_bufferSize != 0) {
		delete[] m_buffer;
		m_bufferSize = 0;
	}
	
	(*m_bufferQueueInterface)->Clear(m_bufferQueueInterface);
}

void SoundPlayer::EnqueueBuffer(int16_t* buffer, size_t bufferSize)
{
	(*m_bufferQueueInterface)->Enqueue(m_bufferQueueInterface,
					   buffer, bufferSize * sizeof(int16_t));
}

uint32_t SoundPlayer::GetSamplingRate()
{
	return m_samplingRate;
}
