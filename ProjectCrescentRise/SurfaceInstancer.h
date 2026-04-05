#pragma once

#include "Library.h"
#include "Noise.h"
#include "GameObject.h"
#include "PlanetSurface.h"
#include <functional>
#include <cstdint>

enum class InstanceType
{
	None, Grass, PickupModel, model, Enemy
};

struct InstancerSettings
{
	InstanceType instanceType = InstanceType::None;

	int duplicateModelAmt = -1;
	int duplicateTextureAmt = -1;

	std::string modelLocation = "./Assets/Mesh/rock.fbx";
	glm::vec3 colour = glm::vec3(1.0f);

	float density = 0.1f;
	float noiseScale = 50.0f;
	float noiseThreshold = 0.3f;
	int noiseSeed = 12345;
	bool useHeightLayerMask = false;
	int heightLayerMask = 0;
	int passesPerFace = 16;

	std::string textureLoc;

	float maxSize = 1.0f;
	float minSize = 1.0f;
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
		InstanceCreatorFunc creator
	);

private:
	InstancerSettings m_settings;
	mutable Noise m_noise;

	float EvaluateNoiseAtPosition(const glm::vec3& worldPos) const;

	int SampleFace(std::shared_ptr<PlanetSurface> planet, InstanceCreatorFunc creator, int samplesPerFace, const glm::vec3& faceNormal);
};
