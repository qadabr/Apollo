#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

#include <cmath>

class Butterworth
{
 public:
	Butterworth(double frequency, double samplingRate, double resonance);
	
	double Update(double newInput);
 private:
	double m_resonance;
	double m_frequency;
	double m_samplingRate;
	double c, a1, a2, a3, b1, b2;

	double m_inputHistory[2] = { 0 };
	double m_outputHistory[3] = { 0 };
};

#endif /* BUTTERWORTH_H */
