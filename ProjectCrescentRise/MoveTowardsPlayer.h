#pragma once

#include "EnemyAbstractState.h"

class MoveTowardsPlayer : public EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager);
	virtual void UpdateState(EnemyStateManager& t_manager);
	virtual void ExitState(EnemyStateManager& t_manager);
private:
	float m_moveSpeed = 5.0f;

	float enemyFollowDetectionRange = 40.0f;
	float enemyAttackRange = 1.0f;
};

