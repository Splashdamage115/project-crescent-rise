#include "PlayerInput.h"
#include "KeyScan.h"
#include "Game.h"
#include "OnlineDispatcher.h"
#include <string>
#include "CommandInterpreter.h"

glm::vec3 PlayerInput::playerPosition;
bool PlayerInput::noClipEnabled = false;

void PlayerInput::Start()
{
	CommandInterpreter::append([this](std::string t) { this->noClipEnabled = !this->noClipEnabled; }, "/NOCLIP");
}

void PlayerInput::Update()
{
	glm::vec3 displacement = glm::vec3(0.0f);
	if (KeyScan::isKeyDown(KeyScan::KeyCode::D))
	{
		displacement.x += currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::S))
	{
		displacement.z += -currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::A))
	{
		displacement.x += -currentSpeed * static_cast<float>(Game::deltaTime);
	}
	if (KeyScan::isKeyDown(KeyScan::KeyCode::W))
	{
		displacement.z += currentSpeed * static_cast<float>(Game::deltaTime);
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

	if (noClipEnabled)
	{
		transform->moveAlongForward(displacement);
	}
	else
	{
		transform->moveAlongForwardPlanet(displacement);
	}

	std::string position;
	position += std::to_string(transform->position.x);
	position += ',';
	position += std::to_string(transform->position.y);
	position += ',';
	position += std::to_string(transform->position.z);
	position += ',';

	OnlineDispatcher::Dispatch(OnlineDispatcher::DispatchType::movement, position);
	
	float xDelta = static_cast<float>(KeyScan::mouseX - lastMouseX);
	float yDelta = static_cast<float>(KeyScan::mouseY - lastMouseY);
	
	transform->rotation.y -= xDelta * 0.05f;
	transform->rotation.x += yDelta * 0.05f;
	
	
	lastMouseY = KeyScan::mouseY;
	lastMouseX = KeyScan::mouseX;

	playerPosition = transform->position;
	
	//std::cout << transform->rotation.x << ", " << transform->rotation.y << ", " << transform->rotation.z << "\n";
	//std::cout << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << "\n";
}
