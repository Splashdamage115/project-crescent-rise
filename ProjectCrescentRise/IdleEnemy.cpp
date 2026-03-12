#include "IdleEnemy.h"
#include "MoveTowardsPlayer.h"
#include "WanderState.h"
#include "Game.h"

void IdleEnemy::EnterState(EnemyStateManager& t_manager)
{
	timeToWanderLeft = timeToWander;
}

void IdleEnemy::UpdateState(EnemyStateManager& t_manager)
{
	timeToWanderLeft -= static_cast<float>(Game::deltaTime);

	if(t_manager.checkPlayerVisibility(enemyDetectionRange))
	{
		t_manager.EnterNewState(std::make_shared<MoveTowardsPlayer>());
		return;
	}
	if (timeToWanderLeft <= 0.0f)
	{
		t_manager.EnterNewState(std::make_shared<WanderState>());
		return;
	}
}

void IdleEnemy::ExitState(EnemyStateManager& t_manager)
{
}
