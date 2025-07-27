#pragma once
#include <JuceHeader.h>

class BitCrushEffect : public osci::EffectApplication {
public:
	// algorithm from https://www.kvraudio.com/forum/viewtopic.php?t=163880
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>>& values, double sampleRate) override {
		double effectScale = juce::jlimit(0.0, 1.0, values[0].load());
		double value = values[1];
		// change rage of value from 0-1 to 0.0-0.78
		double rangedValue = value * 0.78;
		double powValue = std::exp2(1.0 - rangedValue) - 1.0;
		double crush = powValue * 12;
		double x = std::exp2(crush);
		double quant = 0.5 * x;
		double dequant = 1.0f / quant;
		// TODO floor vs round
		osci::Point output(dequant * std::round(input.x * quant),
						   dequant * std::round(input.y * quant),
						   dequant * std::round(input.z * quant));
		//osci::Point output(dequant * (int)(input.x * quant), dequant * (int)(input.y * quant), dequant * (int)(input.z * quant));
		return (1 - effectScale) * input + effectScale * output;
	}
};
