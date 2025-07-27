#pragma once
#include <JuceHeader.h>

class OscirenderAudioProcessor;
class DashedLineEffect : public osci::EffectApplication {
public:
	DashedLineEffect(OscirenderAudioProcessor &p);
	~DashedLineEffect();

	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override;

private:
	OscirenderAudioProcessor &audioProcessor;
	const static int MAX_BUFFER = 192000;
	std::vector<osci::Point> buffer = std::vector<osci::Point>(MAX_BUFFER);
	int bufferIndex = 0;
	double framePhase = 0.0; // [0, 1]
};
