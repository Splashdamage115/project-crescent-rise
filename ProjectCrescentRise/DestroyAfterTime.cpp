#include "DestroyAfterTime.h"
#include "Game.h"

void DestroyAfterTime::Start()
{
	timeAliveLeft = maxTimeAlive;
}

void DestroyAfterTime::Update()
{
	timeAliveLeft -= Game::deltaTime;

	if (timeAliveLeft <= 0.f)
	{
		parent->active = false;
	}
}
