#include "SilverRay.h"

#include "Window.h"

SilverRay::SilverRay(SoundEngine* engine,
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
	m_recorder = new SoundRecorder(m_engine, m_samplingRate);
}

SilverRay::~SilverRay()
{
	delete m_player;
	delete m_recorder;
}

void SilverRay::PushMessage(const std::string& message)
{
	size_t bufferSize;
	int16_t* buffer = GenerateWave(message, &bufferSize);

	m_player->EnqueueBuffer(buffer, bufferSize);
}

void SilverRay::Send()
{
	m_player->Play();
}

static std::string BitsToString(std::string& bits)
{
	std::string result = "";

	for (size_t i = 0; i < bits.size(); i += 8) {
		char c = 0;
		for (size_t j = 0; j < 8; ++j) {
			c |= (bits[i + j] - '0') << (7 - j);
		}

		result += c;
	}

	return result;
}

static bool CompareDouble(double a, double b, double err)
{
	return std::abs(a - b) < err;
}

static bool CompareDouble(double a, double b)
{
	return CompareDouble(a, b, 0.1);
}

void SilverRay::ParseBuffer(std::list<double>& frequencySequence,
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

std::string SilverRay::GetBitList(std::list<double>& frequencySequence, size_t frameStep)
{
	std::string result = "";
	
	size_t bitCount = 0;
	size_t rawSize = 0;
	double prvFrequency = 0;

	auto frequencyIterator = frequencySequence.begin();
	while (frequencyIterator++ != frequencySequence.end()) {
		double curFrequency = *frequencyIterator;
		if (CompareDouble(curFrequency, 0) && frequencyIterator != frequencySequence.end()) {
			continue;
		}
		
		if (CompareDouble(curFrequency, prvFrequency)) {
			++bitCount;
			prvFrequency = curFrequency;
			continue;
		}

		rawSize = bitCount ? 1 + (bitCount - 1) / frameStep : 0;
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

void SilverRay::ReceiveMessage(size_t milliseconds)
{
	m_recorder->Record();
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	m_recorder->Stop();
	
	const size_t frameStep = 4;
	const size_t frameSize = m_duration * m_samplingRate / 1000;
	
	std::vector<short> largeBuffer;
	for (size_t chunkIndex = 0; not m_recorder->IsEmpty(); ++chunkIndex) {
		std::unique_ptr<short[]> chunk(m_recorder->DequeueBuffer());
		largeBuffer.insert(largeBuffer.end(),
				   chunk.get(),
				   chunk.get() + m_recorder->GetBufferSize());
	}

	std::list<double> frequencySequence;
	ParseBuffer(frequencySequence,
		    largeBuffer.data(),
		    largeBuffer.size(),
		    frameSize,
		    frameStep);
	
	std::string bitList = GetBitList(frequencySequence, frameStep);
	printf(" Received: %s\n", bitList.c_str());
	std::string message = BitsToString(bitList);
	printf("  Message: %s\n", message.c_str());
}

inline int GetBit(const std::string& message, size_t index)
{
	return message[index / 8] & (1 << (7 - index) % 8) ? 1 : 0;
}

int16_t* SilverRay::GenerateWave(const std::string& message, size_t* bufferSize)
{
	/* Размер фрагмента волны, который должен звучать на одной высоте */
	size_t fragmentSize = (size_t)(m_duration * m_player->GetSamplingRate() / 1000.0);

	/* Количество битов в байте */
	size_t fragmentCount = 8 * message.length();

	*bufferSize = fragmentSize * fragmentCount;

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
			angle += angularFrequency;

			if (angle > 2 * M_PI) {
				angle -= 2 * M_PI;
			}
		}
	}
	printf("\n");

	return wave;
}

double SilverRay::FrameFrequency(int16_t* buffer, size_t x0, size_t frameN)
{
	FrameFrequencyDetector detector(std::vector<short>(&buffer[x0], &buffer[x0] + frameN),
					m_samplingRate);

	detector.Filter(m_minFreq - 50, m_maxFreq + 50);

	double maxMag = detector.GetMaxMagnitude();
	double magMin = detector.GetMagnitude(m_minFreq) / maxMag * 100; //%
	double magMax = detector.GetMagnitude(m_maxFreq) / maxMag * 100; //%

	if (CompareDouble(magMin, magMax, 50)) {
		return 0;
	}	

	if (CompareDouble(magMin, 100, 50)) {
		return m_minFreq;
	}

	if (CompareDouble(magMax, 100, 50)) {
		return m_maxFreq;
	}
	
	return 0;
}
