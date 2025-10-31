#pragma once

#include "NoiseSettings.h"

class ShapeSettings
{
public:
	struct NoiseLayer
	{
		bool enabled = true;
		NoiseSettings noiseSettings;
		bool useAsMask = true;
	};

	float planetRadius = 1.f;
	std::vector<NoiseLayer> noiseLayers;
};

