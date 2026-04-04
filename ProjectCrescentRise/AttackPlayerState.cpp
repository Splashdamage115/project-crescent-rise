#include "AttackPlayerState.h"
#include "MoveTowardsPlayer.h"
#include "Game.h"

void AttackPlayerState::EnterState(EnemyStateManager& t_manager)
{
	cooldownRemaining = 0.f;
}

void AttackPlayerState::UpdateState(EnemyStateManager& t_manager)
{
	if (cooldownRemaining > 0.f) cooldownRemaining -= static_cast<float>(Game::deltaTime);

	if(t_manager.checkPlayerVisibility(attackDistance) && cooldownRemaining <= 0.f)
	{
		// attack player here

		t_manager.playerObject->sendMessage("DAMAGE", damageDealt);
		t_manager.screenOverlay->sendMessage("DAMAGE");

		cooldownRemaining = maxCooldown;
	}
	else if (!t_manager.checkPlayerVisibility(attackDistance))
	{
		t_manager.EnterNewState(std::make_shared<MoveTowardsPlayer>());
		return;
	}
}

void AttackPlayerState::ExitState(EnemyStateManager& t_manager)
{
	cooldownRemaining = 0.f;
}
