#pragma once

#include "EnemyAbstractState.h"

class IdleEnemy : public EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager);
	virtual void UpdateState(EnemyStateManager& t_manager);
	virtual void ExitState(EnemyStateManager& t_manager);
private:
	float enemyDetectionRange = 30.0f;

	float timeToWander = 15.0f;
	float timeToWanderLeft = 0.0f;
};

