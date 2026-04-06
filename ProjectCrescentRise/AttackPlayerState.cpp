#include "AttackPlayerState.h"
#include "MoveTowardsPlayer.h"
#include "Game.h"

void AttackPlayerState::EnterState(EnemyStateManager& t_manager)
{
	cooldownRemaining = 0.7f;
	lungeTimeLeft = maxLungeTime;
}

void AttackPlayerState::UpdateState(EnemyStateManager& t_manager)
{
	if (cooldownRemaining > 0.f) cooldownRemaining -= static_cast<float>(Game::deltaTime);

	if (cooldownRemaining <= 0.1f)
	{
		t_manager.parent->sendMessage("charging");
	}
	if(t_manager.checkPlayerVisibility(attackDistance) && cooldownRemaining <= 0.f)
	{
		// lunge to player
		glm::vec3 direction = glm::normalize(t_manager.playerPosition - t_manager.getTransform()->position);
		t_manager.getTransform()->position += direction * (float)Game::deltaTime * lungeSpeed;

		lungeTimeLeft -= (float)Game::deltaTime;

		if (lungeTimeLeft <= 0.f)
		{
			lungeTimeLeft = maxLungeTime;
			cooldownRemaining = maxCooldown;
		}

		// attack player here
		if (t_manager.checkPlayerVisibility(0.2f))
		{
			t_manager.playerObject->sendMessage("DAMAGE", damageDealt);
			t_manager.screenOverlay->sendMessage("DAMAGE");
			cooldownRemaining = maxCooldown;
		}
	}
	else if (!t_manager.checkPlayerVisibility(attackDistance) && cooldownRemaining <= 0.f)
	{
		t_manager.EnterNewState(std::make_shared<MoveTowardsPlayer>());
		return;
	}
}

void AttackPlayerState::ExitState(EnemyStateManager& t_manager)
{
	cooldownRemaining = 0.7f;
}
