#include "WanderState.h"
#include "MoveTowardsPlayer.h"
#include "IdleEnemy.h"
#include "Game.h"

void WanderState::EnterState(EnemyStateManager& t_manager)
{
	wanderTimeLeft = maxWanderTime;
}

void WanderState::UpdateState(EnemyStateManager& t_manager)
{
	wanderTimeLeft -= static_cast<float>(Game::deltaTime);

	if (t_manager.checkPlayerVisibility(enemyDetectionRange))
	{
		t_manager.EnterNewState(std::make_shared<MoveTowardsPlayer>());
		return;
	}

	if (wanderTimeLeft <= 0.0f)
	{
		t_manager.EnterNewState(std::make_shared<IdleEnemy>());
		return;
	}

	wanderDirectionAngle += (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * maxDirectionChange;
	glm::vec2 wanderDirection = degreesToDisplacement(wanderDirectionAngle);
	glm::vec3 wanderDirection3d = glm::vec3(wanderDirection.x, 0.0f, wanderDirection.y);
	glm::vec3 wanderMovement = wanderDirection3d * moveSpeed * static_cast<float>(Game::deltaTime);

	t_manager.getTransform()->position += wanderMovement;

	
	
}

void WanderState::ExitState(EnemyStateManager& t_manager)
{

}
