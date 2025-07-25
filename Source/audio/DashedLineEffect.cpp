#include "DashedLineEffect.h"
#include "../PluginProcessor.h"

DashedLineEffect::DashedLineEffect(OscirenderAudioProcessor &p) : audioProcessor(p) {}

DashedLineEffect::~DashedLineEffect() {}

osci::Point DashedLineEffect::apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) {
	// dash length in seconds
	double dashLength = values[0] / 200;
	double dashCount = juce::jmax(1.0, std::round(values[0].load())); // dashes per cycle
	double dashCoverage = juce::jlimit(0.0, 1.0, values[1].load());

	buffer[bufferIndex] = input;

	// TODO: Use circular buffer and non-sample aligned delay oscillator

	dashCount += values[2] / audioProcessor.frequency;
	double trueDashLength = (sampleRate / audioProcessor.frequency) / dashCount; // rename
	delayPhase += 1.0 / (trueDashLength);
	delayPhase = delayPhase - floor(delayPhase);
	//int dashLengthSamples = (int)std::round((sampleRate / audioProcessor.frequency) / dashCount);

	//int dashLengthSamples = (int)(dashLength * sampleRate);
	//dashLengthSamples = juce::jlimit(1, MAX_BUFFER, dashLengthSamples);
	

	// Linear interpolation works much better than nearest for this
	double samplePos = bufferIndex - trueDashLength * delayPhase * (1 - dashCoverage);
	samplePos = samplePos - MAX_BUFFER * std::floor(samplePos / MAX_BUFFER); // wrap to [0, maxBuffer)
	int lowIndex = (int)std::floor(samplePos);
	int highIndex = (lowIndex + 1) % MAX_BUFFER;
	double mixFactor = samplePos - floor(samplePos); // Fractional part
	osci::Point output = (1 - mixFactor) * buffer[lowIndex] + mixFactor * buffer[highIndex];

	bufferIndex++;
	if (bufferIndex >= buffer.size()) {
		bufferIndex = 0;
	}
	return output;
}