#pragma once
#include <JuceHeader.h>

class GodrayEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		double noiseAmp = juce::jlimit(0.0, 1.0, values[0].load());
		double bias = values[1];//juce::jmax(values[1].load(), 0.0);
		double biasExponent = std::pow(4.0, std::abs(bias));

		// bias: < -2, maybe not quite 2?

		double noise = 2.0 * (double)std::rand() / RAND_MAX - 1.0;
		double noiseSign = noise >= 0.0 ? 1.0 : -1.0;
		// If bias is positive, bend values toward 0
		// If bias is negative, bend values toward +/- 1
		if (bias < 0.0) {
			//double bias = 
			noise = noiseSign * (1.0 - std::pow(1.0 - std::abs(noise), biasExponent));
		} else {
			noise = noiseSign * std::pow(std::abs(noise), biasExponent);
		}
		noise = 0.5 + 0.5 * noise;

		double scale = (1.0 - noiseAmp) + noise * noiseAmp;
		return input * scale;
	}
};
