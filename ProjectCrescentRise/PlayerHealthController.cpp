#include "PlayerHealthController.h"

void PlayerHealthController::Start()
{
	maxHealth = 100;
	currentHealth = maxHealth;
}

void PlayerHealthController::Update()
{
	if (dead)
	{
		std::cout << "PLAYER DEAD!\n";
	}
}

void PlayerHealthController::expire()
{
	dead = true;
}
