#ifndef WINDOW_H
#define WINDOW_H

#include <cmath>

class Window
{
 public:
        static double Rectangle(double n, double frameSize)
        {
		return 1;
        }

        static double Gausse(double n, double frameSize)
        {
		auto a = (frameSize - 1) / 2;
		auto t = (n - a) / (0.5 * a);
		t = t * t;
		return std::exp(-t/2);
        }

        static double Hamming(double n, double frameSize)
        {
		return 0.54 - 0.46 * std::cos((2 * M_PI * n) / (frameSize - 1));
        }

        static double Hann(double n, double frameSize)
        {
		return 0.5 * (1 - std::cos((2 * M_PI * n) / (frameSize - 1)));
        }

        static double BlackmannHarris(double n, double frameSize)
        {
		return 0.35875 -
			(0.48829 * std::cos((2 * M_PI * n) / (frameSize - 1))) +
			(0.14128 * std::cos((4 * M_PI * n) / (frameSize - 1))) -
			(0.01168 * std::cos((4 * M_PI * n) / (frameSize - 1)));
        }
};

#endif /* WINDOW_H */
