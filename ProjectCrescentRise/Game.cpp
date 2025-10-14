#include "Game.h"
#include "Update.h"
#include "GameObjects.h"
#include "Mover.h"
#include "billboard2D.h"
#include "Window.h"
#include "CameraFeed.h"
#include "PlayerInput.h"

double Game::deltaTime = 0;

void Game::initGame()
{
	Window::Get();
	Window::Get().InitCamera();
	GameObject obj;
	obj.addScript(std::make_shared<billboard2D>());
	obj.transform->scale = { 300.0f, 300.0f, 1.0f };
	obj.transform->position = { 0.0f, -1.8f, -150.0f };

	obj.transform->rotation = { 90.0f, 0.0f, 0.0f };
	GameObjects::addNewObjectToPool(obj);

	GameObject obj2;
	obj2.transform->position = { 0.0f, 0.0f, 3.0f };
	obj2.transform->rotation = { 0.0f, 0.0f, 0.0f };
	obj2.addScript(std::make_shared<CameraFeed>());
	obj2.addScript(std::make_shared<PlayerInput>());

	GameObjects::addNewObjectToPool(obj2);
}

int Game::playGame()
{
	double currentFrame = 0.0f;
	double lastFrame = 0.0f;

	while (!Window::Get().windowClosed())
	{
		// update deltatime
		currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame; deltaTime = glm::clamp(deltaTime, 0.0, 5.0);

		Update::update();
		// render as often as possible
		Window::Get().render();
	}
	return 0;
}
