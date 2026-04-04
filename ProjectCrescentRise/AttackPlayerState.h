#pragma once

#include "EnemyAbstractState.h"

class AttackPlayerState : public EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager);
	virtual void UpdateState(EnemyStateManager& t_manager);
	virtual void ExitState(EnemyStateManager& t_manager);
private:
	float damageDealt = 10.f;
	float attackDistance = 1.5f;
	float cooldownRemaining = 0.f;
	float maxCooldown = 1.0f;
};
