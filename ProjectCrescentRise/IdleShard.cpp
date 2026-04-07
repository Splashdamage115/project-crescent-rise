#include "IdleShard.h"
#include "AttackPlayerShard.h"

void IdleShard::EnterState(EnemyStateManager& t_manager)
{
}

void IdleShard::UpdateState(EnemyStateManager& t_manager)
{
	if (t_manager.checkPlayerVisibility(enemyDetectionRange))
	{
		t_manager.EnterNewState(std::make_shared<AttackPlayerShard>());
		return;
	}
}

void IdleShard::ExitState(EnemyStateManager& t_manager)
{
}
