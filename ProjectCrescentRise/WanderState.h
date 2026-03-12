#pragma once

#include "EnemyAbstractState.h"

class WanderState : public EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager);
	virtual void UpdateState(EnemyStateManager& t_manager);
	virtual void ExitState(EnemyStateManager& t_manager);

	float maxWanderTime = 15.0f;
private:
	float moveSpeed = 3.0f;

	float wanderDirectionAngle = 0.0f;
	float maxDirectionChange = 3.0f;

	float enemyDetectionRange = 30.0f;
	float wanderTimeLeft = 0.0f;
};

