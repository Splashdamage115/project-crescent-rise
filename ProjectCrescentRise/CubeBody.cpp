#include "CubeBody.h"
#include "Game.h"

void CubeBody::Update()
{
	transform->position.x -= 0.2 * Game::deltaTime;

	std::cout << transform->position.x << std::endl;
}
