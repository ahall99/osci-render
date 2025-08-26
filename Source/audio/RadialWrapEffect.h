#pragma once
#include <JuceHeader.h>

class RadialWrapEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		// TODO: Make angle relative to top
		// Treat input as complex number and raise to integer power
		// Disallowing non-integer and negative exponents because of the branch cut
		double effectScale = juce::jlimit(0.0, 1.0, values[0].load());
		double exponent = juce::jmax(1.0, std::round(values[1].load()));
		double r2 = input.x * input.x + input.y * input.y;
		double theta = std::atan2(input.y, input.x);

		double outR = std::pow(r2, 0.5 * exponent);
		double outTheta = exponent * theta;
		osci::Point output(outR * std::cos(outTheta), outR * std::sin(outTheta), input.z);
		return (1 - effectScale) * input + effectScale * output;
	}
};
