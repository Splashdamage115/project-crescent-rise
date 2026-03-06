#include "EnemyMovement.h"
#include "Game.h"
#include "PlayerInput.h"
#include "Raycast.h"


void EnemyMovement::Start()
{
}

void EnemyMovement::Update()
{
	if (!moving) return;

	glm::vec3 playerPos = PlayerInput::playerPosition;
	
	glm::vec3 enemyPos = transform->position;

	if (glm::distance(enemyPos, playerPos) < 30.f)
	{
		Ray ray = Ray::CastTowardsPosition(enemyPos, playerPos);
		ray.hitsphere(playerPos, 1.0f, ray.distance);
		if(ray.gameObject == nullptr)
		{
			canSeePlayer = true;
		}
		else
		{
			canSeePlayer = false;
		}
	}
	else
	{
		canSeePlayer = false;
	}

	if(canSeePlayer)
	{
		glm::vec3 direction = glm::normalize(playerPos - enemyPos);
		transform->position += direction * (float)Game::deltaTime * m_moveSpeed;
		//transform->rotation.y = displacementToDegrees(direction);
	}
}
