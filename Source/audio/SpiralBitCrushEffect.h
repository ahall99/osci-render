#pragma once
#include <JuceHeader.h>
#include <numbers>

class SpiralBitCrushEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		double effectScale = juce::jlimit(0.0, 1.0, values[0].load());
		double domainX = juce::jmax(2.0, std::round(values[1].load()));
		double domainY = std::round(values[2].load());
		double offsetX = values[3].load();
		double offsetY = -values[4].load(); // Negate so positive offset zooms in instead of out

		osci::Point output(0, 0, input.z);
		if (input.x != 0 || input.y != 0) {
			double domainHypot = std::hypot(domainX, domainY);
			double domainTheta = std::atan2(domainY, domainX);
			double scale = domainHypot / (2 * std::numbers::pi);

			// Convert input point to log-polar coordinates transformed based on domain and offset
			double r = std::hypot(input.x, input.y);
			double logR = std::log(r);
			double theta = std::atan2(input.y, input.x);
			double logPolarX = theta * std::cos(domainTheta) - logR * std::sin(domainTheta);
			double logPolarY = theta * std::sin(domainTheta) + logR * std::cos(domainTheta);
			logPolarX = scale * logPolarX + offsetX;
			logPolarY = scale * logPolarY + offsetY;

			// Round this point to the center of the log-polar cell the input lies in, convert back to cartesian
			double cellX = std::round(logPolarX);
			double cellY = std::round(logPolarY);
			double outTheta = (cellX - offsetX) / scale;
			double outLogR = (cellY - offsetY) / scale;
			double newOutTheta = outTheta * std::cos(domainTheta) + outLogR * std::sin(domainTheta);
			double newOutLogR = -outTheta * std::sin(domainTheta) + outLogR * std::cos(domainTheta);
			double outR = std::exp(newOutLogR);
			double outX = outR * std::cos(newOutTheta);
			double outY = outR * std::sin(newOutTheta);
			output.x = (1 - effectScale) * input.x + effectScale * outX;
			output.y = (1 - effectScale) * input.y + effectScale * outY;
		}
		return output;
	}
};
