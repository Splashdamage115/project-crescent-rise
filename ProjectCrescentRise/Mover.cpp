#include "Mover.h"
#include "Game.h"

void Mover::Update()
{
	transform->moveAlongForward(velocity * static_cast<float>(Game::deltaTime));
	transform->rotation += rotation * static_cast<float>(Game::deltaTime);
	transform->scale += scale * static_cast<float>(Game::deltaTime);


	//std::cout << transform->position.x << "/" << transform->position.y << "/" << transform->position.z << "\n";
}
