#include "Game.h"
#include "Update.h"
#include "GameObjects.h"
#include "CubeBody.h"

double Game::deltaTime = 0;

void Game::initGame()
{
	GameObject obj;
	obj.addScript(std::make_shared<CubeBody>());
	GameObjects::addNewObjectToPool(obj);
}

int Game::playGame()
{
	double currentFrame = 0.0f;
	double lastFrame = 0.0f;

	while (!m_window.windowClosed())
	{
		// update deltatime
		currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame; glm::clamp(deltaTime, 0.0, 5.0);

		Update::update();
		// render as often as possible
		m_window.render();
	}
	return 0;
}
