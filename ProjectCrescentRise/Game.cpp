#include "Game.h"
#include "Update.h"
#include "GameObjects.h"
#include "Mover.h"
#include "billboard2D.h"

double Game::deltaTime = 0;

void Game::initGame()
{
	GameObject obj;
	std::shared_ptr<Mover> mover = std::make_shared<Mover>();
	mover->velocity = glm::vec3(0.0f, 0.0f, 0.5f);
	obj.addScript(mover);
	obj.addScript(std::make_shared<billboard2D>());
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
