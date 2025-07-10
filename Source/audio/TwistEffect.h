#pragma once
#include <JuceHeader.h>
#include <numbers>

class TwistEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		double twistStrength = values[0] * std::numbers::pi;
		double twistTheta = twistStrength * input.y;
		double newX = input.x * std::cos(twistTheta) - input.z * std::sin(twistTheta);
		double newZ = input.x * std::sin(twistTheta) + input.z * std::cos(twistTheta);
		return osci::Point(newX, input.y, newZ);
	}
};
