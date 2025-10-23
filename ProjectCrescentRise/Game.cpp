#include "Game.h"
#include "Update.h"
#include "Mover.h"
#include "billboard2D.h"
#include "Window.h"
#include "CameraFeed.h"
#include "PlayerInput.h"
#include "GroundTile.h"

double Game::deltaTime = 0;

void Game::initGame()
{
	Window::Get();
	Window::Get().InitCamera();

	camObj = std::make_shared<GameObject>();
	camObj->transform->position = { -46.3333f, 175.656f, -39.0139f };
	camObj->transform->rotation = { -36.0f, 135.0f, 0.0f };
	camObj->addScript(std::make_shared<CameraFeed>());
	//camObj.addScript(std::make_shared<PlayerInput>());

	GameObjects::addNewObjectToPool(camObj);

	waterObj = std::make_shared<GameObject>();
	waterObj->addScript(std::make_shared<billboard2D>());
	waterObj->transform->rotation = { 90.0f, 0.0f, 0.0f };
	waterObj->transform->position = { 0.0f, 11.0f, 0.0f };
	waterObj->transform->scale = { 3000.0f, 3000.0f, 3000.0f };
	GameObjects::addNewObjectToPool(waterObj);



	initFloor();

	mski = std::make_shared<mouseKeyInput>();
	mski->active = true;
	mski->function = { [this]() {this->initFloor(); } };
	mski->keyCode = KeyScan::MouseKeyCode::RightMouse;
	KeyScan::append(mski, true);
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

void Game::initFloor()
{
	std::shared_ptr<GameObject> floorObj2 = std::make_shared<GameObject>();
	floorObj2->transform->position = { 0.0f, -2.0f, -2.0f };
	floorObj2->transform->scale = { 1.0f, 1.0f, 1.0f };
	floorObj2->addScript(std::make_shared<GroundTile>());
	if(floorObj != nullptr) floorObj->active = false;
	floorObj = floorObj2;
	GameObjects::addNewObjectToPool(floorObj2);
}
