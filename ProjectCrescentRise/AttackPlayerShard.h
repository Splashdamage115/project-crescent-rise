#pragma once

#include "EnemyAbstractState.h"
#include "Library.h"

class AttackPlayerShard : public EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager);
	virtual void UpdateState(EnemyStateManager& t_manager);
	virtual void ExitState(EnemyStateManager& t_manager);

	void spawnBullet(glm::vec3 startPosition, glm::vec3 targetPosition);
private:
	float visibleRange = 35.0f;

	int bulletsToFireLeft = 3;
	int burstAmount = 3;
	float burstCooldown = 0.f;
	float maxBurstCooldown = 0.3f;
	float shootCooldown = 0.f;
	float maxShootCooldown = 3.0f;

	float bulletSpeed = 25.0f;
};