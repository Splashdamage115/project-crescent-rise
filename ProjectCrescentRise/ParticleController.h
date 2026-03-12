#pragma once

#include "RenderableObject.h"
#include "Particle.h"

class ParticleController : public RenderableObject
{
public:
	static std::shared_ptr<Particle> SpawnNewParticle(std::string textureLocation, Transform overridePosition, float timeAlive, glm::vec2 frameAmt, glm::vec2 textureSize);
	void Update() override;
	void Render() override;
private:
	static std::vector<std::shared_ptr<Particle>> m_particles;
};

