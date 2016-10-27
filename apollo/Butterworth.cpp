#include "Butterworth.h"

Butterworth::Butterworth(double frequency, double samplingRate, double resonance)
	: m_resonance(resonance),
	  m_frequency(frequency),
	  m_samplingRate(samplingRate)
{
	c = 1.0 / std::tan(M_PI * frequency / m_samplingRate);
	a1 = 1.0 / (1.0 + resonance * c + c * c);
	a2 = -2.0 * a1;
	a3 = a1;
	b1 = 2.0 * (c * c - 1.0) * a1;
	b2 = (1.0 - resonance * c + c * c) * a1;
}

double Butterworth::Update(double newInput)
{
	double newOutput = a1 * newInput
		+ a2 * m_inputHistory[0]
		+ a3 * m_inputHistory[1]
		- b1 * m_outputHistory[0]
		- b2 * m_outputHistory[1];

        m_inputHistory[1] = m_inputHistory[0];
        m_inputHistory[0] = newInput;

        m_outputHistory[2] = m_outputHistory[1];
        m_outputHistory[1] = m_outputHistory[0];
        m_outputHistory[0] = newOutput;

	return m_outputHistory[0];
}
