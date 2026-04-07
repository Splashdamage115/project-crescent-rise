#include "PlayerHealthController.h"
#include "GameObjects.h"
#include"CommandInterpreter.h"
#include "Game.h"

void PlayerHealthController::Start()
{
	// gui will enable god mode automatically
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/GUI");

	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/GODMODE");
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/godmode");
	CommandInterpreter::append([this](std::string t) { this->EnableGodMode(t); }, "/Godmode");

	maxHealth = 100;
	currentHealth = maxHealth;

	screenOverlay = nullptr;

	auto t = GameObjects::getAllOfTag("overlay");

	if (t.size() > 0)
		screenOverlay = t.at(0);
}

void PlayerHealthController::Update()
{
	if (invincibilityTime >= 0.f)
		invincibilityTime -= static_cast<float>(Game::deltaTime);
	if (dead)
	{
		std::cout << "PLAYER DEAD!\n";
	}
}

void PlayerHealthController::expire()
{
	if (godMode) return;

	dead = true;



	screenOverlay->sendMessage("DEAD");

}

void PlayerHealthController::secondaryHitEffect()
{
	invincibilityTime = 0.1f;
	screenOverlay->sendMessage("DAMAGE");
}

void PlayerHealthController::EnableGodMode(std::string _)
{
	godMode = !godMode;
}
