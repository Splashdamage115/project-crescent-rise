#include "PlayerInput.h"
#include "KeyScan.h"
#include "Game.h"

void PlayerInput::Update()
{
	glm::vec3 displacement = glm::vec3(0.0f);
	if (KeyScan::isKeyDown(KeyScan::KeyCode::D))
	{
		displacement.x += currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::S))
	{
		displacement.z += currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::A))
	{
		displacement.x += -currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::W))
	{
		displacement.z += -currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::Q))
	{
		displacement.y += -currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::E))
	{
		displacement.y += currentSpeed * static_cast<float>(Game::deltaTime);
	}

	if (KeyScan::isKeyDown(KeyScan::KeyCode::LSHIFT))
	{
		currentSpeed += speedIncrease * static_cast<float>(Game::deltaTime);
		if (currentSpeed >= maxSpeed) currentSpeed = maxSpeed;
	}
	else
	{
		currentSpeed = minSpeed;
	}

	transform->moveAlongForward(displacement);

	float xDelta = static_cast<float>(KeyScan::mouseX - lastMouseX);
	float yDelta = static_cast<float>(KeyScan::mouseY - lastMouseY);

	transform->rotation.y += xDelta * 0.05f;
	transform->rotation.x -= yDelta * 0.05f;


	lastMouseY = KeyScan::mouseY;
	lastMouseX = KeyScan::mouseX;
}
