#include "PlayerHealthController.h"
#include "GameObjects.h"

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

	std::shared_ptr<GameObject> screenOverlay = nullptr;

	auto t = GameObjects::getAllOfTag("overlay");

	if (t.size() > 0)
		screenOverlay = t.at(0);

	screenOverlay->sendMessage("DEAD");

}
