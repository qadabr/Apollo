#include "SilverPush.h"

SilverPush::SilverPush(uint32_t samplingRate, double minFreq, double maxFreq, size_t duration)
	: m_minFreq(minFreq),
	  m_maxFreq(maxFreq),
	  m_duration(duration),
	  m_samplingRate(samplingRate)
{
	m_engine = new SoundEngine();
	m_player = new SoundPlayer(m_engine, samplingRate);
	m_recorder = new SoundRecorder(m_engine, samplingRate);
}

SilverPush::~SilverPush()
{
	delete m_recorder;
	delete m_player;
	delete m_engine;
}

void SilverPush::SendMessage(const std::string& message)
{
	size_t bufferSize;
	char* buffer = this->generateWave(message, &bufferSize);

	m_player->ClearQueue();
	m_player->EnqueueBuffer(buffer, bufferSize);
	m_player->Play();
}

std::string SilverPush::ReceiveMessage()
{
	std::string message = "";

	
	
	return message;
}

static char getTetrit(const std::string& message, size_t index)
{
	/* Выделяем тетрит из байта */
	char tetrit = (message[index / 4] >> ((index % 4) * 2)) & 0x3;
	LOG_D("Message: \"%s\", char 0x%2x, tribit with index %u is 0x%1x\n",
	      message.c_str(), message[index / 4], index, tetrit);
	
	return tetrit;
}

char* SilverPush::generateWave(const std::string& message, size_t* bufferSize)
{
	/* Размер фрагмента волны, который должен звучать на одной высоте */
	size_t fragmentSize = (size_t)(m_duration * m_player->GetSamplingRate() / 1000.0);

	/* Количество тетритов в байте */
	size_t fragmentCount = 4 * message.length();
	
	/* Синтезируемая волна */
	double* wave = new double[fragmentSize * fragmentCount];

	size_t x = 0;
	for (size_t i = 0; i < fragmentCount; ++i) {
		double freq = getTetrit(message, i) * (m_maxFreq - m_minFreq) / 3.0 + m_minFreq;
		LOG_D("%0.2f Hz\n", freq);
		for (size_t j = 0; j < fragmentSize; ++j) {
			double value = 1.0 * x * freq / m_player->GetSamplingRate();
			// Синусоида
			wave[x++] = 32767 * sin(2 * M_PI * value);
			// Пила
			// wave[x++] = 32767 * (fmod(value, 1.0) * 2 - 1);
		}
	}

	/* Конвертируем в массив байт */
	*bufferSize = fragmentSize * fragmentCount * 2;
	char* buffer = new char[*bufferSize];
	
	size_t j = 0;
	for (size_t i = 0; i < fragmentSize * fragmentCount; ++i) {
		int16_t val = (int16_t)wave[i];
		buffer[j++] = (val >> 0) & 0xff;
		buffer[j++] = (val >> 8) & 0xff; 
	}

	delete[] wave;
	return buffer;
}

double SilverPush::frameFrequency(int16_t* buffer, size_t pointA, size_t pointB)
{
	// Размер фрейма в дискретах
	size_t frameN = pointB - pointA;

	// Количество изменений знака функции на протяжении фрейма
	size_t n = 0;
	
	for (size_t i = pointA; i < pointB - 1; ++i) {
		// Получим на выходе число со знаком, если изначально знаки у них были разные
		if (buffer[i] ^ buffer[i + 1] < 0) {
			++n;
		}
	}

	// Средняя частота функции в фрейме
	return 2.0 * m_samplingRate * frameN / n;
}
