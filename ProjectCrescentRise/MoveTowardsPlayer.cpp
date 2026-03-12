#include "MoveTowardsPlayer.h"
#include "Game.h"

void MoveTowardsPlayer::EnterState(EnemyStateManager& t_manager)
{
}

void MoveTowardsPlayer::UpdateState(EnemyStateManager& t_manager)
{
	glm::vec3 direction = glm::normalize(t_manager.playerPosition - t_manager.getTransform()->position);
	t_manager.getTransform()->position += direction * (float)Game::deltaTime * m_moveSpeed;
}

void MoveTowardsPlayer::ExitState(EnemyStateManager& t_manager)
{
}
