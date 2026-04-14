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
	if (t_manager.parent.expired()) return;

	if (cooldownRemaining > 0.f) cooldownRemaining -= static_cast<float>(Game::deltaTime);

	if (cooldownRemaining <= 0.1f)
	{
		if (!t_manager.parent.expired() && t_manager.parent.lock()->active)
			t_manager.parent.lock()->sendMessage("charging");
	}
	if(t_manager.checkPlayerVisibility(attackDistance) && cooldownRemaining <= 0.f)
	{
		
		glm::vec3 direction = glm::normalize(t_manager.playerPosition - t_manager.getTransform()->position);
		t_manager.getTransform()->position += direction * (float)Game::deltaTime * lungeSpeed;

		lungeTimeLeft -= (float)Game::deltaTime;

		if (lungeTimeLeft <= 0.f)
		{
			lungeTimeLeft = maxLungeTime;
			cooldownRemaining = maxCooldown;
		}

		
		if (t_manager.checkPlayerVisibility(0.2f))
		{
			t_manager.playerObject->sendMessage("DAMAGE", damageDealt);
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
