#include "PlayerInput.h"
#include "KeyScan.h"
#include "Game.h"

void PlayerInput::Update()
{
	glm::vec3 displacement = glm::vec3(0.0f);
	if (KeyScan::isKeyDown(KeyScan::KeyCode::D))
	{
		displacement.x += 10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::S))
	{
		displacement.z += 10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::A))
	{
		displacement.x += -10.0f * Game::deltaTime;
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::W))
	{
		displacement.z += -10.0f * Game::deltaTime;
	}

	transform->moveAlongForward(displacement);

	float xDelta = KeyScan::mouseX - lastMouseX;
	float yDelta = KeyScan::mouseY - lastMouseY;

	transform->rotation.y += xDelta * 0.05f;
	transform->rotation.x -= yDelta * 0.05f;


	lastMouseY = KeyScan::mouseY;
	lastMouseX = KeyScan::mouseX;

	//std::cout << "x: " << KeyScan::mouseX << ", y: " << KeyScan::mouseY << "\n";
}
