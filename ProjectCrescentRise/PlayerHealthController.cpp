#include "PlayerHealthController.h"
#include "GameObjects.h"
#include"CommandInterpreter.h"

void PlayerHealthController::Start()
{
	// gui will enable god mode automatically
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/GUI");

	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/GODMODE");
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/godmode");
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/Godmode");

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
	if (godMode) return;

	dead = true;

	std::shared_ptr<GameObject> screenOverlay = nullptr;

	auto t = GameObjects::getAllOfTag("overlay");

	if (t.size() > 0)
		screenOverlay = t.at(0);

	screenOverlay->sendMessage("DEAD");

}

void PlayerHealthController::EnableGodMode(std::string _)
{
	godMode = !godMode;
}
