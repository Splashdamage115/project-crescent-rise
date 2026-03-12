#include "MoveTowardsPlayer.h"
#include "Game.h"
#include "IdleEnemy.h"

void MoveTowardsPlayer::EnterState(EnemyStateManager& t_manager)
{
}

void MoveTowardsPlayer::UpdateState(EnemyStateManager& t_manager)
{
	if (!t_manager.checkPlayerVisibility(enemyFollowDetectionRange))
	{
		t_manager.EnterNewState(std::make_shared<IdleEnemy>());
	}

	glm::vec3 direction = glm::normalize(t_manager.playerPosition - t_manager.getTransform()->position);
	t_manager.getTransform()->position += direction * (float)Game::deltaTime * m_moveSpeed;
}

void MoveTowardsPlayer::ExitState(EnemyStateManager& t_manager)
{
}
