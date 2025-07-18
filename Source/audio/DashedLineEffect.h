#pragma once
#include <JuceHeader.h>

class DashedLineEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>>& values, double sampleRate) override {
		// dash length in seconds
		double dashLength = values[0] / 200;
		double dashCoverage = juce::jlimit(0.0, 1.0, values[1].load());
		int dashLengthSamples = (int)(dashLength * sampleRate);
		dashLengthSamples = juce::jlimit(1, MAX_BUFFER, dashLengthSamples);
		buffer[bufferIndex] = input;

		// Linear interpolation works much better than nearest for this
		double samplePos = dashCoverage * bufferIndex;
		int lowIndex = (int)std::floor(samplePos);
		int highIndex = std::min(lowIndex + 1, dashLengthSamples - 1);
		double mixFactor = samplePos - floor(samplePos); // Fractional part
		osci::Point output = (1 - mixFactor) * buffer[lowIndex] + mixFactor * buffer[highIndex];

		bufferIndex++;
		if (bufferIndex >= dashLengthSamples) {
			bufferIndex = 0;
		}
		return output;
	}

private:
	const static int MAX_BUFFER = 192000;
	std::vector<osci::Point> buffer = std::vector<osci::Point>(MAX_BUFFER);
	int bufferIndex = 0;
};
