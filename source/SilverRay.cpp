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

void SilverRay::SignalFiltration(int16_t* buffer, size_t bufferSize, double filterFreq)
{
	size_t tempSize = 2;
	while (tempSize < bufferSize) {
		tempSize *= 2;
	}
	
	std::vector<double> temp(tempSize);
	std::copy(buffer, buffer + bufferSize, temp.begin());
	
	auto fft = Aquila::FftFactory::getFft(tempSize);
	auto spectrum = fft->fft(temp.data());

	Aquila::SpectrumType filterSpectrum(tempSize);
	for (std::size_t i = 0; i < tempSize; ++i) {
		double freq1 = tempSize * filterFreq / m_samplingRate;
		double freq2 = tempSize * (m_samplingRate - filterFreq) / m_samplingRate; 
		
		if (i < freq1 || i > freq2) {
			filterSpectrum[i] = 0.0;
		}
		else {
			filterSpectrum[i] = 2.0;
		}
	}

	std::transform(std::begin(spectrum),
		       std::end(spectrum),
		       std::begin(filterSpectrum),
		       std::begin(spectrum),
		       [] (Aquila::ComplexType x, Aquila::ComplexType y) { return x * y; }
	);

	fft->ifft(spectrum, temp.data());

	std::copy(temp.begin(), temp.begin() + bufferSize, buffer);
}

static bool CompareDouble(double a, double b, double err)
{
	if (std::abs(a - b) < err) {
		return true;
	}

	return false;
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
		//double minA, maxA;
		//GetFrequency(buffer, bufferIndex, frameSize, minA, maxA);

		//LOG_I("%f - %f", minA, maxA);
		
		double frequency = FrameFrequency(buffer, bufferIndex, frameSize); 
		frequencySequence.push_back(frequency);

		bufferIndex += frameSize / frameStep;
	}
}

void SilverRay::FilterFrequencySequence(std::list<double>& frequencySequence)
{
	for (double& frequency : frequencySequence) {
		if (std::abs(frequency - m_minFreq) < 100) {
			frequency = m_minFreq;
			continue;
		}
		
		if (std::abs(frequency - m_maxFreq) < 100) {
			frequency = m_maxFreq;
			continue;
		}

		frequency = 0;
	}
}

std::string SilverRay::GetBitList(std::list<double>& frequencySequence, size_t frameStep)
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

template <typename T>
static void SmoothSignal(T input[], T output[], int n, int window)
{
	int i,j,z,k1,k2,hw;
	double tmp;
	
	if (fmod(window, 2) == 0)
		window++;

	hw = (window - 1) / 2;
	output[0] = input[0];
	
	for (i = 1; i < n; i++) {
		tmp = 0;
		if (i < hw) {
			k1 = 0;
			k2 = 2 * i;
			z = k2 + 1;
		}
		else if ((i + hw) > (n - 1)) {
			k1 = i - n + i + 1;
			k2 = n - 1;
			z = k2 - k1 + 1;
		}
		else {
			k1 = i - hw;
			k2 = i + hw;
			z = window;
		}
		
		for (j = k1;j <= k2;j++) {
			tmp = tmp + input[j];
		}
		
		output[i] = tmp / z;
	}
}

void SilverRay::ReceiveMessage(size_t milliseconds)
{
	m_recorder->Record();
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	
	const size_t frameStep = 4;
	const size_t frameSize = m_duration * m_samplingRate / 1000;
	
	std::vector<short> largeBuffer(m_recorder->Size() * m_recorder->GetBufferSize());
	for (size_t chunkIndex = 0; not m_recorder->IsEmpty(); ++chunkIndex) {
		std::unique_ptr<short[]> chunk(m_recorder->DequeueBuffer());

		std::list<double> frequencySequence;
		ParseBuffer(frequencySequence,
			    chunk.get(),
			    m_recorder->GetBufferSize(),
			    frameSize,
			    frameStep);
		
		FilterFrequencySequence(frequencySequence);
		std::string bitList = GetBitList(frequencySequence, frameStep);

		printf(" Received: %s\n", bitList.c_str());
	}

	m_recorder->Stop();
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

double SilverRay::FrameFrequency(int16_t* buffer, size_t x0, size_t frameN)
{
	FrameFrequencyDetector detector(std::vector<short>(&buffer[x0], &buffer[x0] + frameN),
					m_samplingRate);

	detector.HighFilter(2 * m_minFreq - m_maxFreq);
	
	double maxMag = detector.GetMaxMagnitude();
	double magMin = detector.GetMagnitude(m_minFreq) / maxMag * 100; //%
	double magMax = detector.GetMagnitude(m_maxFreq) / maxMag * 100; //%

	double freq = 0;
	if (CompareDouble(magMin, 100, 50)) {
		freq = m_minFreq;
	}

	if (CompareDouble(magMax, 100, 50)) {
		freq = m_maxFreq;
	}
	
	return freq;
}
