#pragma once

#include "Library.h"
#include "Noise.h"
#include "GameObject.h"
#include "PlanetSurface.h"
#include <functional>

struct InstancerSettings
{
	float density = 0.1f;           
	float noiseScale = 50.0f;
	float noiseThreshold = 0.3f;
	int noiseSeed = 12345;
};

using InstanceCreatorFunc = std::function<std::shared_ptr<GameObject>()>;

class SurfaceInstancer
{
public:
	SurfaceInstancer() = default;

	void SetSettings(const InstancerSettings& settings) { m_settings = settings; }
	InstancerSettings GetSettings() const { return m_settings; }

	void SetNoiseSeed(int seed) { m_settings.noiseSeed = seed; }

	void InstantiateOnSurface(
		std::shared_ptr<PlanetSurface> planet,
		InstanceCreatorFunc creator,
		int samplesPerFace = 64
	);

private:
	InstancerSettings m_settings;
	mutable Noise m_noise;

	float EvaluateNoiseAtPosition(const glm::vec3& worldPos) const;

	int SampleFace(
		std::shared_ptr<PlanetSurface> planet,
		InstanceCreatorFunc creator,
		int samplesPerFace,
		const glm::vec3& faceNormal
	);
};
