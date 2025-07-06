#pragma once
#include <JuceHeader.h>
#include <cmath>
#include <cstdlib>

class GodrayEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		double noiseAmp = juce::jlimit(0.0, 1.0, values[0].load());
		double bias = juce::jmax(values[1].load(), 0.0);
		bias = std::pow(4.0, bias);

		double noise = (double)std::rand() / RAND_MAX;
		noise = std::pow(noise, bias);

		double scale = 1.0 + noiseAmp * noise;
		return input * scale;
	}
};
