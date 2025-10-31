#pragma once

#include "Library.h"
#include "Noise.h"

class NoiseFilter
{
public:
	void init(NoiseSettings t_settings)
	{
		settings = t_settings;
	}
	float Evaluate(glm::vec3 point)
	{
		float noiseValue = 0;
		float frequency = settings.baseRoughness;
		float amplitude = 1;

		for (int i = 0; i < settings.numLayers; i++)
		{
			float v = noise.Evaluate(point * frequency + settings.center);
			noiseValue += (v + 1) * 0.5f * amplitude;
			frequency *= settings.roughness;
			amplitude *= settings.persistence;
		}

		noiseValue = glm::max(0.f, noiseValue - settings.minValue);
		return noiseValue * settings.strength;
	}
private:
	Noise noise;
	NoiseSettings settings;
};

