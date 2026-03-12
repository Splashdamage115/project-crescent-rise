#include "EnemyStateManager.h"
#include "PlayerInput.h"
#include "IdleEnemy.h"

void EnemyStateManager::Start()
{
	EnterNewState(std::make_shared<IdleEnemy>());
}

void EnemyStateManager::Update()
{
	if (!m_currentState) return;
	playerPosition = PlayerInput::playerPosition;

	m_currentState->UpdateState(*this);
}

void EnemyStateManager::EnterNewState(std::shared_ptr<EnemyAbstractState> newState)
{
	if (!newState) return;

	if(m_currentState) m_currentState->ExitState(*this);
	m_currentState = newState;
	m_currentState->EnterState(*this);
}
