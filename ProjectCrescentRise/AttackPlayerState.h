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
	float attackDistance = 4.0f;
	float cooldownRemaining = 0.7f;
	float maxCooldown = 3.0f;
	float lungeSpeed = 15.0f;
};
