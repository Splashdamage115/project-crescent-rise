#include "Mover.h"
#include "Game.h"

void Mover::Update()
{
	transform->position += velocity * static_cast<float>(Game::deltaTime);

	std::cout << transform->position.z << std::endl;
}
