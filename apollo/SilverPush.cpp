#include "SilverPush.h"

SilverPush::SilverPush(double minFreq, double maxFreq, size_t duration)
	: m_minFreq(minFreq),
	  m_maxFreq(maxFreq),
	  m_duration(duration)
{
	m_engine = new SoundEngine();
	m_player = new SoundPlayer(m_engine, 48000);
}

SilverPush::~SilverPush()
{
	delete m_player;
	delete m_engine;
}

void SilverPush::SendMessage(const std::string& message)
{
	size_t bufferSize = (size_t)(4 * message.length() * m_duration
				     * m_player->GetSamplingRate() / 1000.0);
	char* buffer = this->generateWave(message);

	m_player->ClearQueue();
	m_player->EnqueueBuffer(buffer, bufferSize);
	m_player->Play();
}

static char getNibble(const std::string& message, size_t index)
{
	/* Выделяем ниббл из байта */
	char nibble = (message[index / 2] >> ((index % 2) * 4)) & 0xf;
	LOG_D("Message: \"%s\", char 0x%2x, nibble with index %u is 0x%1x\n",
	      message.c_str(), message[index / 2], index, nibble);
	
	return nibble;
}

char* SilverPush::generateWave(const std::string& message)
{
	/* Размер фрагмента волны, который должен звучать на одной высоте */
	size_t fragmentSize = (size_t)(m_duration * m_player->GetSamplingRate() / 1000.0);

	/* Количество фрагментов */
	size_t fragmentCount = 2 * message.length();
	
	/* Синтезируемая волна */
	double* wave = new double[fragmentSize * fragmentCount];

	size_t x = 0;
	for (size_t i = 0; i < fragmentCount; ++i) {
		double freq = getNibble(message, i) * (m_maxFreq - m_minFreq) / 0xf + m_minFreq;
		LOG_D("%0.2f Hz\n", freq);
		for (size_t j = 0; j < fragmentSize; ++j) {
			double value = 1.0 * x * freq / m_player->GetSamplingRate();
			wave[x++] = 32767 * sin(2 * M_PI * value);
		}
	}

	/* Конвертируем в массив байт */
	char* buffer = new char[fragmentSize * fragmentCount * 2];
	
	size_t j = 0;
	for (size_t i = 0; i < fragmentSize * fragmentCount; ++i) {
		short int val = (short int)wave[i];
		buffer[j++] = (val >> 0) & 0xff;
		buffer[j++] = (val >> 8) & 0xff; 
	}

	delete[] wave;
	return buffer;
}
