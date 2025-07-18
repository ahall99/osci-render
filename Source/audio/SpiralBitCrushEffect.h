#pragma once
#include <JuceHeader.h>
#include <numbers>

class SpiralBitCrushEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		// Completing one revolution in input space traverses the hypotenuse of one "domain" in log-polar space
		double effectScale = juce::jlimit(0.0, 1.0, values[0].load());
		double domainX = juce::jmax(2.0, std::round(values[1].load())); // TODO try r, theta user controls for domain
		double domainY = std::round(values[2].load());
		osci::Point offset(values[3].load(), -values[4].load());

		osci::Point output(0, 0, input.z);
		if (input.x != 0 || input.y != 0) {
			double domainHypot = std::hypot(domainX, domainY);
			double domainTheta = std::atan2(domainY, domainX);
			double scale = domainHypot / (2 * std::numbers::pi);

			// Convert input point to log-polar coordinates transformed based on domain and offset
			double r = std::hypot(input.x, input.y);
			double logR = std::log(r);
			double theta = std::atan2(input.y, input.x);
			osci::Point logPolarCoords(theta, logR);
			logPolarCoords.rotate(0, 0, domainTheta);
			logPolarCoords = logPolarCoords * scale + offset;

			// Round this point to the center of the log-polar cell the input lies in, convert back to cartesian
			logPolarCoords.x = std::round(logPolarCoords.x);
			logPolarCoords.y = std::round(logPolarCoords.y);
			logPolarCoords = (logPolarCoords - offset) * (1 / scale);
			logPolarCoords.rotate(0, 0, -domainTheta);
			double outR = std::exp(logPolarCoords.y);
			double outTheta = logPolarCoords.x;
			output = osci::Point(outR * std::cos(outTheta), outR * std::sin(outTheta), input.z);
		}
		return (1 - effectScale) * input + effectScale * output;
	}
};
