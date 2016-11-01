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
	m_player = new SoundPlayer(m_engine, m_samplingRate);
	m_recorder = new SoundRecorder(m_engine, m_samplingRate, 4);
}

SilverPush::~SilverPush()
{
	delete m_player;
	delete m_recorder;
}

void SilverPush::PushMessage(const std::string& message)
{
	size_t bufferSize;
	int16_t* buffer = GenerateWave(message, &bufferSize);

	m_player->EnqueueBuffer(buffer, bufferSize);
}

void SilverPush::Send()
{
	m_player->Play();
}

static std::string BitsToString(std::vector<bool> &bits)
{
	std::string result = "";

	for (size_t i = 0; i < bits.size(); i += 8) {
		char c = 0;
		for (size_t j = 0; j < 8; ++j) {
			c |= bits[i + j] << (7 - j);
		}

		result += c;
	}

	return result;
}

void SilverPush::SignalFiltration(int16_t* buffer, size_t bufferSize, double filterFreq)
{
	std::vector<double> temp(bufferSize);
	
	auto fft = Aquila::FftFactory::getFft(bufferSize);
	auto spectrum = fft->fft(temp.data());
	fft->ifft(spectrum, temp.data());

	for (size_t i = 0; i < bufferSize; ++i) {
		buffer[i] = (int16_t)temp[i];
	}
}

static bool CompareDouble(double a, double b)
{
	if (std::abs(a - b) < 0.1) {
		return true;
	}

	return false;
}

void SilverPush::ParseBuffer(std::list<double>& frequencySequence,
			     int16_t* buffer,
			     size_t bufferSize,
			     size_t frameSize,
			     size_t frameStep)
{
	for (size_t bufferIndex = 0; bufferIndex < bufferSize - frameSize; ) {
		double frequency = FrameFrequency(buffer, bufferIndex, frameSize); 
		frequencySequence.push_back(frequency);

		bufferIndex += frameSize / frameStep;
	}
}

void SilverPush::FilterFrequencySequence(std::list<double>& frequencySequence)
{
	for (double& frequency : frequencySequence) {
		if (std::abs(frequency - m_minFreq) < 50) {
			frequency = m_minFreq;
			continue;
		}

		if (std::abs(frequency - m_maxFreq) < 50) {
			frequency = m_maxFreq;
			continue;
		}

		frequency = 0;
	}
}

std::string SilverPush::GetBitList(std::list<double>& frequencySequence, size_t frameStep)
{
	std::string result = "";
	
	size_t bitCount = 0;
	double prvFrequency = 0;

	auto frequencyIterator = frequencySequence.begin();
	while (frequencyIterator++ != frequencySequence.end()) {
		double curFrequency = *frequencyIterator;
		
		if (CompareDouble(curFrequency, prvFrequency)) {
			++bitCount;
			prvFrequency = curFrequency;
			continue;
		}

		size_t rawSize = bitCount ? 1 + (bitCount - 1) / frameStep : 0;
		if (rawSize > 0) {
			bitCount = 0;
		}

		while (rawSize--) {
			if (CompareDouble(prvFrequency, m_minFreq)) {
				result += '0';
				continue;
			}
			
			if (CompareDouble(prvFrequency, m_maxFreq)) {
				result += '1';
				continue;
			}
		}

		prvFrequency = curFrequency;
	}

	return result;
}

void SilverPush::ReceiveMessage()
{
	m_recorder->Record();
	
	const size_t frameStep = 250;
	const size_t frameSize = m_duration * m_samplingRate / 1000;
	const size_t bufferSize = m_recorder->GetBufferSize();

	while (true) {
		int16_t* buffer = m_recorder->DequeueBuffer();
		if (buffer == nullptr) {
			continue;
		}

		std::list<double> frequencySequence;
		ParseBuffer(frequencySequence, buffer, bufferSize, frameSize, frameStep);
		FilterFrequencySequence(frequencySequence);
		std::string bitList = GetBitList(frequencySequence, frameStep);

		printf(" Received: %s\n", bitList.c_str());

		delete[] buffer;
	}

	m_recorder->Stop();
}

inline int GetBit(const std::string& message, size_t index)
{
	return message[index / 8] & (1 << (7 - index) % 8) ? 1 : 0;
}

int16_t* SilverPush::GenerateWave(const std::string& message, size_t* bufferSize)
{
	/* Размер фрагмента волны, который должен звучать на одной высоте */
	size_t fragmentSize = (size_t)(m_duration * m_player->GetSamplingRate() / 1000.0);

	/* Количество битов в байте */
	size_t fragmentCount = 8 * message.length();

	*bufferSize = fragmentSize * fragmentCount * 2;

	/* Синтезируемая волна */
	int16_t* wave = new int16_t[*bufferSize];

	size_t x = 0;
	double angle = 0;

	printf("Generated: ");
	for (size_t i = 0; i < fragmentCount; ++i) {
		double freq = GetBit(message, i) * (m_maxFreq - m_minFreq) + m_minFreq;
		printf("%u", GetBit(message, i));

		for (size_t j = 0; j < fragmentSize; ++j) {
			double angularFrequency = 2 * M_PI * freq / m_player->GetSamplingRate();

			wave[x++] = (int16_t)(32767 * sin(angle));
			wave[x++] = (int16_t)(32767 * sin(angle));
			angle += angularFrequency;

			if (angle > 2 * M_PI) {
				angle -= 2 * M_PI;
			}
		}
	}
	printf("\n");

	return wave;
}

double SilverPush::FrameFrequency(int16_t* buffer, size_t x0, size_t frameN)
{
	// Количество изменений знака функции на протяжении фрейма
	size_t n = 0;

	for (size_t i = x0; i < x0 + frameN; ++i) {
		// Получим на выходе число со знаком, если изначально знаки у них были разные
		if (buffer[i] * buffer[i + 1] < 0.0) {
			++n;
		}
	}

	// Средняя частота функции в фрейме
	return 0.5 * m_samplingRate * n / frameN;
}
