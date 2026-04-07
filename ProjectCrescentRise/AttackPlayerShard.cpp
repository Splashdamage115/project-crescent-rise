#include "AttackPlayerShard.h"
#include "IdleShard.h"
#include "Game.h"
#include "GameObjects.h"

#include "Mover.h"
#include "DestroyAfterTime.h"
#include "CubeSphere.h"
#include "PlanetSurface.h"

void AttackPlayerShard::EnterState(EnemyStateManager& t_manager)
{
	shootCooldown = maxShootCooldown;
}

void AttackPlayerShard::UpdateState(EnemyStateManager& t_manager)
{
	if (!t_manager.checkPlayerVisibility(visibleRange))
	{
		t_manager.EnterNewState(std::make_shared<IdleShard>());
		return;
	}
	burstCooldown -= Game::deltaTime;
	if (burstCooldown <= 0.f && bulletsToFireLeft > 0)
	{

		// adjust start position of the bullet to be higher
		glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		if (PlanetSurface::s_instance && PlanetSurface::s_instance->getTransform())
		{
			upDirection = glm::normalize(t_manager.getTransform()->position - PlanetSurface::s_instance->getTransform()->position);
		}
		glm::vec3 adjustedStartPosition = t_manager.getTransform()->position + upDirection * 1.0f;

		glm::vec3 adjustedTargetPosition = t_manager.playerPosition + upDirection * 0.8f;


		spawnBullet(adjustedStartPosition, adjustedTargetPosition);

		bulletsToFireLeft--;
		burstCooldown = maxBurstCooldown;
	}
	if (bulletsToFireLeft < 0)
	{
		shootCooldown -= Game::deltaTime;
	}
	if (shootCooldown <= 0.f)
	{
		shootCooldown = maxShootCooldown;
		bulletsToFireLeft = burstAmount;
	}
}

void AttackPlayerShard::ExitState(EnemyStateManager& t_manager)
{
}


void AttackPlayerShard::spawnBullet(glm::vec3 startPosition, glm::vec3 targetPosition)
{
	std::shared_ptr<GameObject> bullet = std::make_shared<GameObject>();

	bullet->transform->position = startPosition;
	bullet->transform->scale = glm::vec3(0.2f);

	auto moverScript = std::make_shared<Mover>();

	moverScript->moveAlongV = true;
	moverScript->velocity = glm::normalize(targetPosition - startPosition) * bulletSpeed;

	bullet->addScript(moverScript);

	auto destroyAfter = std::make_shared<DestroyAfterTime>();
	destroyAfter->maxTimeAlive = 5.0f;
	bullet->addScript(destroyAfter);

	bullet->addScript(std::make_shared<CubeSphere>());

	GameObjects::addNewObjectToPool(bullet);
}
