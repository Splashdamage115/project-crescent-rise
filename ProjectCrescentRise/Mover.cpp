#include "Mover.h"
#include "Game.h"

void Mover::Update()
{
	transform->position += velocity * static_cast<float>(Game::deltaTime);
	transform->rotation += rotation * static_cast<float>(Game::deltaTime);
	transform->scale += scale * static_cast<float>(Game::deltaTime);
}
