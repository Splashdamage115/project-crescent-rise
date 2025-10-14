#include "PlayerInput.h"
#include "KeyScan.h"
#include "Game.h"

void PlayerInput::Update()
{
	if (KeyScan::isKeyDown(KeyScan::KeyCode::D))
	{
		transform->position.x += 10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::S))
	{
		transform->position.z += 10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::A))
	{
		transform->position.x += -10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::W))
	{
		transform->position.z += -10.0f * Game::deltaTime;
	}

	float xDelta = KeyScan::mouseX - lastMouseX;
	float yDelta = KeyScan::mouseY - lastMouseY;

	transform->rotation.y += xDelta * 0.05f;
	transform->rotation.x -= yDelta * 0.05f;


	lastMouseY = KeyScan::mouseY;
	lastMouseX = KeyScan::mouseX;

	//std::cout << "x: " << KeyScan::mouseX << ", y: " << KeyScan::mouseY << "\n";
}
