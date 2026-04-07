#include "EnemyStateManager.h"
#include "PlayerInput.h"
#include "IdleEnemy.h"
#include "Raycast.h"
#include "GameObjects.h"
#include "IdleShard.h"

void EnemyStateManager::Start()
{
	if(m_enemyType == EnemyType::Slime)
		EnterNewState(std::make_shared<IdleEnemy>());
	else if (m_enemyType == EnemyType::Shard)
		EnterNewState(std::make_shared<IdleShard>());
}

void EnemyStateManager::Update()
{
	if (!parent->active) return;
	if (!m_currentState) return;
	playerPosition = PlayerInput::playerPosition;

	m_currentState->UpdateState(*this);

	if (playerObject == nullptr)
	{
		auto t = GameObjects::getAllOfTag("Player");

		if (t.size() > 0)
			playerObject = t.at(0);
	}

	if (screenOverlay == nullptr)
	{
		auto t = GameObjects::getAllOfTag("overlay");

		if (t.size() > 0)
			screenOverlay = t.at(0);
	}
}

void EnemyStateManager::EnterNewState(std::shared_ptr<EnemyAbstractState> newState)
{
	if (!parent->active) return;
	if (!newState) return;

	if(m_currentState) m_currentState->ExitState(*this);
	m_currentState = newState;
	m_currentState->EnterState(*this);
}

bool EnemyStateManager::checkPlayerVisibility(float t_distance)
{
	if (!parent->active) return false;

	glm::vec3 playerPos = playerPosition;

	glm::vec3 enemyPos = getTransform()->position;

	if (glm::distance(enemyPos, playerPos) < t_distance)
	{
		Ray ray = Ray::CastTowardsPosition(enemyPos, playerPos);
		ray.hitsphere(playerPos, 1.0f, ray.distance);

		if (ray.gameObject == nullptr)
		{
			return true;
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
	return false;
}
