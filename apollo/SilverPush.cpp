#include "SilverPush.h"

SilverPush::SilverPush(double minFreq, double maxFreq, size_t duration)
	: m_minFreq(minFreq),
	  m_maxFreq(maxFreq),
	  m_duration(duration)
{
	m_engine = new SoundEngine();
	m_player = new SoundPlayer(m_engine);
}

SilverPush::~SilverPush()
{
	delete m_player;
	delete m_engine;
}

void SilverPush::SendMessage(const std::string& message)
{
	size_t bufferSize = (size_t)(4 * message.length() * m_duration * SAMPLING_RATE / 1000.0);
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
	size_t fragmentSize = (size_t)(m_duration * SAMPLING_RATE / 1000.0);

	/* Количество фрагментов */
	size_t fragmentCount = 2 * message.length();
	
	/* Синтезируемая волна */
	double* wave = new double[fragmentSize * fragmentCount];

	size_t x = 0;
	for (size_t i = 0; i < fragmentCount; ++i) {
		double freq = getNibble(message, i) * (m_maxFreq - m_minFreq) / 0xf + m_minFreq;
		for (size_t j = 0; j < fragmentSize; ++j) {
			wave[x] = sin(2 * M_PI * x / SAMPLING_RATE * freq);
			++x;
		}
	}

	/* Конвертируем в массив байт */
	char* buffer = new char[fragmentSize * fragmentCount * 2];
	
	size_t j = 0;
	for (size_t i = 0; i < fragmentSize * fragmentCount; ++i) {
		short val = (short)(wave[i] * 32767);
		buffer[j++] = (char)(val * 0x00ff);
		buffer[j++] = (char)((val * 0xff00) >> 8); 
	}

	delete[] wave;
	return buffer;
}
