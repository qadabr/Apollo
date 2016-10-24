#include "SilverPush.h"

SilverPush::SilverPush(SoundEngine* engine,
		       uint32_t samplingRate,
		       double minFreq,
		       double maxFreq,
		       size_t duration)
	: m_minFreq(minFreq),
	  m_maxFreq(maxFreq),
	  m_duration(duration),
	  m_samplingRate(samplingRate),
	  m_engine(engine)
{
	m_player = new SoundPlayer(m_engine, samplingRate);
}

SilverPush::~SilverPush()
{
	delete m_player;
}

void SilverPush::PushMessage(const std::string& message)
{
	size_t bufferSize;
	char* buffer = this->generateWave(message, &bufferSize);

	m_player->EnqueueBuffer(buffer, bufferSize);
}

void SilverPush::Send()
{
	m_player->Play();
}

void addByteToMessage(std::string& message, char* messageByte)
{
	char result = 0;
	for (size_t i = 0; i < 8; ++i) {
		result |= messageByte[i] << (7 - i);
	}

	message += result;
}

double SilverPush::frequencyFilter(double freq)
{
	double error = 300;
	
	if (std::abs(freq - m_minFreq) <= error) {
		return m_minFreq;
	}

	if (std::abs(freq - m_maxFreq) <= error) {
		return m_maxFreq;
	}

	return 0.0;
}

enum FREQ { ZERO, MIN_FREQ, MAX_FREQ, FREQ_SIZE};

inline size_t bitsInFrame(size_t hits, size_t frameStep)
{
	return hits ? 1 + (hits - 1) / frameStep : 0;
}

void SilverPush::ReceiveMessage()
{
	m_recorder = new SoundRecorder(m_engine, m_samplingRate);
	m_recorder->ClearQueue();
	m_recorder->Record();

	const size_t frameN = m_duration * m_samplingRate / 1000;

       	// Предыдущая частота
	double lastFreq = 0.0;

	// Шаг смещения в частях окна
	size_t frameStep = 4;

	// Количество подряд фиксаций одной и той же частоты
	size_t hits[FREQ_SIZE] = { 0 };

	while (true) {
		int16_t* buffer = (int16_t*)m_recorder->DequeueBuffer();
		size_t bufferSize = m_recorder->GetBufferSize() / sizeof(int16_t);
		
		if (buffer == nullptr) {
			continue;
		}

		for (size_t i = 0; i < bufferSize - frameN; ) {
			double freq = frequencyFilter(frameFrequency(buffer, i, frameN));
			LOG_D("%f", freq);
			
			if (std::abs(freq - m_minFreq) < 0.1) {
				hits[ZERO] = 0;
				++hits[MIN_FREQ];
			}

			if (std::abs(freq - m_maxFreq) < 0.1) {
				hits[ZERO] = 0;
				++hits[MAX_FREQ];
			}

			// Если наткнулись на 0
			if (std::abs(freq) < 0.1) {
				for (size_t i = 0; i < bitsInFrame(hits[MIN_FREQ], frameStep); ++i) {
					printf("%u", 0);
				}

				for (size_t i = 0; i < bitsInFrame(hits[MAX_FREQ], frameStep); ++i) {
					printf("%u", 1);
				}

				hits[MIN_FREQ] = hits[MAX_FREQ] = 0;
				++hits[ZERO];
			}

			lastFreq = freq;
			i += frameN / frameStep;
		}

		// Ряд в пять нулей подряд говорит о том, что, скорее всего, принимать нечего
		if (hits[ZERO] >= 5) {
			break;
		}

		delete[] buffer;
	}	

	printf("\n");
	delete m_recorder;
}

inline int getBit(const std::string& message, size_t index)
{
	return message[index / 8] & (1 << (7 - index) % 8) ? 1 : 0;
}

char* SilverPush::generateWave(const std::string& message, size_t* bufferSize)
{
	/* Размер фрагмента волны, который должен звучать на одной высоте */
	size_t fragmentSize = (size_t)(m_duration * m_player->GetSamplingRate() / 1000.0);

	/* Количество битов в байте */
	size_t fragmentCount = 8 * message.length();
	
	/* Синтезируемая волна */
	double* wave = new double[fragmentSize * fragmentCount];

	printf("Generated: ");
	size_t x = 0;
	for (size_t i = 0; i < fragmentCount; ++i) {
		double freq = getBit(message, i) * (m_maxFreq - m_minFreq) + m_minFreq;
		//LOG_D("%0.2f Hz\n", freq);
		printf("%u", getBit(message, i));
		for (size_t j = 0; j < fragmentSize; ++j) {
			double value = 1.0 * x * freq / m_player->GetSamplingRate();
			// Синусоида
			wave[x++] = 32767 * sin(2 * M_PI * value);
		}
	}

	printf("\n");

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

double SilverPush::frameFrequency(int16_t* buffer, size_t x0, size_t frameN)
{
	// Размер фрейма в дискретах
	size_t x1 = x0 + frameN;

	// Количество изменений знака функции на протяжении фрейма
	size_t n = 0;
	
	for (size_t i = x0; i < x0 + frameN; ++i) {
		// Получим на выходе число со знаком, если изначально знаки у них были разные
		if (buffer[i] * buffer[i + 1] < 0) {
			++n;
		}
	}

	// Средняя частота функции в фрейме
	return 0.5 * m_samplingRate * n / frameN;
}
