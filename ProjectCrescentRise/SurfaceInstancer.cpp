#include "SurfaceInstancer.h"
#include "GameObjects.h"
#include "SurfaceAttacher.h"
#include <iostream>
#include <random>

void SurfaceInstancer::InstantiateOnSurface(
	std::shared_ptr<PlanetSurface> planet,
	InstanceCreatorFunc creator,
	int samplesPerFace
)
{
	if (!planet || !creator)
		return;

	m_noise.SetSeed(m_settings.noiseSeed);

	const glm::vec3 faceNormals[] = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	int totalInstances = 0;
	for (int faceIdx = 0; faceIdx < 6; ++faceIdx)
	{
		int faceInstances = SampleFace(planet, creator, samplesPerFace, faceNormals[faceIdx]);
		totalInstances += faceInstances;
	}

}

int SurfaceInstancer::SampleFace(std::shared_ptr<PlanetSurface> planet,InstanceCreatorFunc creator,int samplesPerFace,const glm::vec3& faceNormal)
{
	if (!planet || !creator)
		return 0;

	int instanceCount = 0;

	glm::vec3 arbitrary = glm::vec3(0.0f, 0.0f, 1.0f);
	if (glm::abs(glm::dot(faceNormal, arbitrary)) > 0.999f)
		arbitrary = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 right = glm::normalize(glm::cross(faceNormal, arbitrary));
	glm::vec3 down = glm::normalize(glm::cross(right, faceNormal));

	float step = 2.0f / (samplesPerFace - 1);
	glm::vec3 planetCenter = planet->getTransform()->position;
	float planetRadius = 1.0f;

	std::mt19937 rng(m_settings.noiseSeed);
	std::uniform_real_distribution<float> jitterDist(-0.4f, 0.4f);

	for (int y = 0; y < samplesPerFace; ++y)
	{
		for (int x = 0; x < samplesPerFace; ++x)
		{
			float baseU = -1.0f + x * step;
			float baseV = -1.0f + y * step;

			float jitterU = jitterDist(rng) * step;
			float jitterV = jitterDist(rng) * step;

			float u = glm::clamp(baseU + jitterU, -1.0f, 1.0f);
			float v = glm::clamp(baseV + jitterV, -1.0f, 1.0f);

			glm::vec3 pointOnUnitSphere = faceNormal + u * right + v * down;
			pointOnUnitSphere = glm::normalize(pointOnUnitSphere);

			glm::vec3 worldSamplePoint = planetCenter + pointOnUnitSphere * planetRadius;

			glm::vec3 surfacePoint = planet->GetSurfacePointFromWorldPosition(worldSamplePoint);

			if (m_settings.useHeightLayerMask)
			{
				int layerIndex = planet->GetHeightLayerIndexFromWorldPosition(surfacePoint);
				if (((m_settings.heightLayerMask >> layerIndex) & 1u) == 0u)
					continue;
			}

			float noiseValue = EvaluateNoiseAtPosition(surfacePoint);

			noiseValue = (noiseValue + 1.0f) * 0.5f;

			if (noiseValue < m_settings.noiseThreshold)
			{
				float rand = glm::fract(glm::sin(glm::dot(surfacePoint, glm::vec3(12.9898f, 78.233f, 45.164f))) * 43758.5453f);
				if (rand < m_settings.density)
				{
					std::shared_ptr<GameObject> instance = creator();
					if (instance)
					{
						instance->transform->position = surfacePoint;

						auto attacher = std::make_shared<SurfaceAttacher>();
						attacher->SetPlanetSurface(planet);
						instance->addScript(attacher);

						instance->active = true;
						GameObjects::addNewObjectToPool(instance);
						instanceCount++;
					}
				}
			}
		}
	}

	return instanceCount;
}

float SurfaceInstancer::EvaluateNoiseAtPosition(const glm::vec3& worldPos) const
{
	glm::vec3 scaledPos = worldPos * (1.0f / m_settings.noiseScale);
	return m_noise.Evaluate(scaledPos);
}
