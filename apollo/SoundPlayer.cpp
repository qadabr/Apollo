#include "SoundPlayer.h"

SoundPlayer::SoundPlayer()
{
	createEngine();
	getIidEngine();
	createOutputMix();
	createAudioPlayer();
	getIidPlay();
	getIidBufferQueue();
}

void SoundPlayer::PlayTone(double freq, double duration)
{
	size_t bufferSize = (size_t)(44100 * duration * 2);
	char* buffer = new char[bufferSize];

	for (size_t i = 0; i < bufferSize; ) {
		short val = (short)(sin(2.0 * M_PI * i / 44100 * freq) * 32768);
		buffer[i++] = (char)(val & 0x00ff);
		buffer[i++] = (char)((val & 0xff00) >> 8);
	}

	(*m_bufferQueue)->Clear(m_bufferQueue);
	(*m_bufferQueue)->Enqueue(m_bufferQueue, buffer, bufferSize);

	sleep(duration);
	//	delete buffer;
}

void SoundPlayer::createEngine()
{
	const SLInterfaceID pIDs[] = { SL_IID_ENGINE };
	const SLboolean pIDsRequired[]  = { SL_BOOLEAN_TRUE };

	SLresult result = slCreateEngine(&m_engineObj,
					 0, NULL,
					 1, pIDs,
					 pIDsRequired);
	result = (*m_engineObj)->Realize(m_engineObj, SL_BOOLEAN_FALSE);
}

void SoundPlayer::getIidEngine()
{
	SLresult result = (*m_engineObj)->GetInterface(m_engineObj,
						       SL_IID_ENGINE,
						       &m_engine);
}

void SoundPlayer::createOutputMix()
{
	const SLInterfaceID pOutputMixIDs[] = {};
	const SLboolean pOutputMixRequired[] = {};

	SLresult result = (*m_engine)->CreateOutputMix(m_engine,
						       &m_outputMixObj,
						       0, pOutputMixIDs,
						       pOutputMixRequired);
	result = (*m_outputMixObj)->Realize(m_outputMixObj, SL_BOOLEAN_FALSE);
}

void SoundPlayer::createAudioPlayer()
{
	SLDataLocator_AndroidSimpleBufferQueue locatorBufferQueue = {
		SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 1
	};
	
	SLDataFormat_PCM formatPCM = {
		SL_DATAFORMAT_PCM,  1, SL_SAMPLINGRATE_44_1,
		SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
		SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN
	};
	
	SLDataSource audioSrc = {
		&locatorBufferQueue,
		&formatPCM
	};
	
	SLDataLocator_OutputMix locatorOutMix = {
		SL_DATALOCATOR_OUTPUTMIX,
		m_outputMixObj
	};
	
	SLDataSink audioSnk = { &locatorOutMix, NULL };
	const SLInterfaceID pIDs[] = { SL_IID_BUFFERQUEUE };
	const SLboolean pIDsRequired[] = { SL_BOOLEAN_TRUE };

	SLresult result = (*m_engine)->CreateAudioPlayer(m_engine,
					      &m_playerObj,
					      &audioSrc,
					      &audioSnk,
					      1, pIDs,
					      pIDsRequired);
	
	result = (*m_playerObj)->Realize(m_playerObj, SL_BOOLEAN_FALSE);
}

void SoundPlayer::getIidPlay()
{
	SLresult result = (*m_playerObj)->GetInterface(m_playerObj, SL_IID_PLAY, &m_player);	
}

void SoundPlayer::getIidBufferQueue()
{
	SLresult result = (*m_playerObj)->GetInterface(m_playerObj,
						       SL_IID_BUFFERQUEUE,
						       &m_bufferQueue);
	result = (*m_player)->SetPlayState(m_player, SL_PLAYSTATE_PLAYING);
}
