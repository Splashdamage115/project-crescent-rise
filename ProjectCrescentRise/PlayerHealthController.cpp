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

void PlayerHealthController::changeHealth(float amount)
{
	currentHealth += amount;

	std::cout << currentHealth << " / player health\n";

	if (currentHealth > maxHealth) currentHealth = maxHealth;
	if (currentHealth <= 0.f)
	{
		currentHealth = 0.f;
		expire();
	}
}
