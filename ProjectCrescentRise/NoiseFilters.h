#pragma once

#include "Library.h"
#include "Noise.h"
#include "NoiseSettings.h"

class NoiseBaseClass
{
public:
	void init(NoiseSettings t_settings)
	{
		settings = t_settings;
	}

	virtual float Evaluate(glm::vec3 point) = 0;
protected:
	Noise noise;
	NoiseSettings settings;
};

class SimpleNoiseFilter : public NoiseBaseClass
{
public:
	virtual float Evaluate(glm::vec3 point) override
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
};


class RigidNoiseFilter : public NoiseBaseClass
{
public:
	virtual float Evaluate(glm::vec3 point) override
	{
		float noiseValue = 0;
		float frequency = settings.baseRoughness;
		float amplitude = 1;
		float weight = 1;

		for (int i = 0; i < settings.numLayers; i++)
		{
			float v = 1 - (glm::abs(noise.Evaluate(point * frequency + settings.center)));
			v *= v;
			v *= weight;
			weight = v * settings.weightMultiplier;
			noiseValue += v * amplitude;
			frequency *= settings.roughness;
			amplitude *= settings.persistence;
		}

		noiseValue = glm::max(0.f, noiseValue - settings.minValue);
		return noiseValue * settings.strength;
	}
};

class NoiseFilterFactory
{
public:
	static std::shared_ptr<NoiseBaseClass> getFilter(NoiseSettings t_settings)
	{
		switch (t_settings.filterType)
		{
		case NoiseSettings::FilterType::Simple:
			return std::make_shared<SimpleNoiseFilter>();
		case NoiseSettings::FilterType::Rigid:
			return std::make_shared<RigidNoiseFilter>();
		default:
			return std::make_shared<SimpleNoiseFilter>();
			break;
		}
	}
};