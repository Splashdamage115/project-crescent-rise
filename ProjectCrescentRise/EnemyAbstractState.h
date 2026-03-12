#pragma once

#include "Library.h"
#include "EnemyStateManager.h"

class EnemyAbstractState
{
public:
	virtual void EnterState(EnemyStateManager& t_manager) = 0;
	virtual void UpdateState(EnemyStateManager& t_manager) = 0;
	virtual void ExitState(EnemyStateManager& t_manager) = 0;

private:

};

