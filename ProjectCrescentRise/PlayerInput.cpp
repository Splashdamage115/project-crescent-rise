#include "PlayerInput.h"
#include "KeyScan.h"
#include "Game.h"
#include "OnlineDispatcher.h"
#include <string>
#include "CommandInterpreter.h"
#include "Raycast.h"

glm::vec3 PlayerInput::playerPosition;
bool PlayerInput::noClipEnabled = false;

void PlayerInput::Start()
{
	CommandInterpreter::append([this](std::string t) { this->noClipEnabled = !this->noClipEnabled; }, "/NOCLIP");

	std::shared_ptr<keyboardInput> ki = std::make_shared<keyboardInput>();
	ki->active = true;
	ki->keyCode = KeyScan::KeyCode::F;
	ki->function = ([this]() { this->handleInteractRelease(); });
	KeyScan::append(ki, true);
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
	if (KeyScan::isKeyDown(KeyScan::KeyCode::LCTRL))
	{
		if(noClipEnabled)
			displacement.y += -currentSpeed * static_cast<float>(Game::deltaTime);
		else
			displacement.y += -0.8f;
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
		displacement *= -1.f;
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
	
	std::vector<std::shared_ptr<GameObject>> go = GameObjects::getAllOfTag("interactible");


	Ray ray = Ray::FromCamera();

	float hitDist = 999999.0f;
	float closestHit = 99999999.0f;
	int closestNum = -1;

	for (int i = 0; i < go.size(); i++)
	{
		glm::vec3 enemyPos = go.at(i)->transform->position;
		if (ray.hitsphere(enemyPos, 1.0f, hitDist))
		{
			if (hitDist < closestHit)
			{
				glm::vec3 hitPoint = ray.at(hitDist);
				closestHit = hitDist;
				closestNum = i;
			}
		}
	}

	if (closestNum != -1)
	{
		if (go.at(closestNum) != highlightedObj)
		{
			if(highlightedObj) *highlightedObj->highlighted = false;
			highlightedObj = go.at(closestNum);
			*highlightedObj->highlighted = true;
		}
	}
	else
	{
		if (highlightedObj) *highlightedObj->highlighted = false;
		highlightedObj = nullptr;
	}
	//std::cout << "Hit enemy at distance " << closestHit << "\n";

	//std::cout << transform->rotation.x << ", " << transform->rotation.y << ", " << transform->rotation.z << "\n";
	//std::cout << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << "\n";
}

void PlayerInput::handleInteractRelease()
{
	if (highlightedObj)
	{
		highlightedObj->sendMessage("PICKUP");
		highlightedObj->active = false;
		highlightedObj = nullptr;
	}
}
