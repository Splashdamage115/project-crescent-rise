#include "IdleEnemy.h"
#include "Raycast.h"
#include "MoveTowardsPlayer.h"

void IdleEnemy::EnterState(EnemyStateManager& t_manager)
{
}

void IdleEnemy::UpdateState(EnemyStateManager& t_manager)
{
	glm::vec3 playerPos = t_manager.playerPosition;

	glm::vec3 enemyPos = t_manager.getTransform()->position;

	if (glm::distance(enemyPos, playerPos) < enemyDetectionRange)
	{
		Ray ray = Ray::CastTowardsPosition(enemyPos, playerPos);
		ray.hitsphere(playerPos, 1.0f, ray.distance);

		if (ray.gameObject == nullptr)
		{
			t_manager.EnterNewState(std::make_shared<MoveTowardsPlayer>());
		}
		else
		{
			// cant see player because something is in the way
		}
	}
	else
	{
		// player outside of detection range
	}
}

void IdleEnemy::ExitState(EnemyStateManager& t_manager)
{
}
