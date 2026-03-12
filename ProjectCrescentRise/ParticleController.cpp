#include "ParticleController.h"

std::vector<std::shared_ptr<Particle>> ParticleController::m_particles;

std::shared_ptr<Particle> ParticleController::SpawnNewParticle(std::string textureLocation, Transform overridePosition, float timeAlive, glm::vec2 frameAmt, glm::vec2 textureSize)
{
	std::shared_ptr<Particle> newParticle;

	for (auto& particle : m_particles)
	{
		if (particle != nullptr && !particle->active)
		{
			newParticle = particle;
			break;
		}
	}
	if (newParticle == nullptr)
	{
		newParticle = std::make_shared<Particle>();
		newParticle->Start();
		m_particles.push_back(newParticle);
	}

	newParticle->active = true;

	newParticle->positionOverride = std::make_shared<Transform>(overridePosition);
	newParticle->textureLocation = textureLocation;
	newParticle->maxTimeAlive = timeAlive;
	newParticle->frameAmt = frameAmt;
	newParticle->textureSize = textureSize;

	newParticle->initialiseData();

	return newParticle;
}

void ParticleController::Update()
{
    for (auto& particle : m_particles)
    {
        if (particle != nullptr && particle->active)
            particle->Update();
    }
}

void ParticleController::Render()
{
	for (auto& particle : m_particles)
    {
		if (particle != nullptr && particle->active)
            particle->Render();
    }
}
