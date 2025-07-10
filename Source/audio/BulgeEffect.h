#pragma once
#include <JuceHeader.h>

class BulgeEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>>& values, double sampleRate) override {
		double value = values[0].load();
		double translatedBulge = -value + 1;

		double r = std::hypot(input.x, input.y);
		double rn = std::pow(r, translatedBulge);
		double scale = rn / r;
		
		return osci::Point(scale * input.x, scale * input.y, input.z);
	}
};
