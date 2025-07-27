#pragma once
#include <osci_render_core/osci_Util.h>
#include <JuceHeader.h>
#include <numbers>

// Inspired by xenontesla122
class PolygonBitCrushEffect : public osci::EffectApplication {
public:
	osci::Point apply(int index, osci::Point input, const std::vector<std::atomic<double>> &values, double sampleRate) override {
		double effectScale = juce::jlimit(0.0, 1.0, values[0].load());
		double nSides = juce::jmax(2.0, values[1].load());
		double bandSize = juce::jmax(1e-4, values[2].load()); // todo exp massage input
		double thetaOffset = values[3] * (2 * std::numbers::pi);// / nSides;
		double rOffset = values[4];

		osci::Point output(0.0, 0.0, input.z);
		if (input.x != 0.0 || input.y != 0.0)
		{
			// TODO is there anything interesting to do with z?
			double r = std::hypot(input.x, input.y);
			double theta = std::atan2(input.y, input.x) -0.5 * std::numbers::pi - thetaOffset;
			theta = osci::Util::wrapAngle(theta + std::numbers::pi) - std::numbers::pi; // Move branch cut after thetaOffset is applied
			double regionTheta = std::round(theta * nSides / (2 * std::numbers::pi)) / nSides * (2 * std::numbers::pi);
			double localTheta = theta - regionTheta;
			double dist = r * std::cos(localTheta);
			double newDist = juce::jmax(0.0, (std::round(dist / bandSize - rOffset) + rOffset) * bandSize);
			double scale = newDist / dist;
			output = osci::Point(scale * input.x, scale * input.y, input.z);
		}

		return (1 - effectScale) * input + effectScale * output;
	}
};
