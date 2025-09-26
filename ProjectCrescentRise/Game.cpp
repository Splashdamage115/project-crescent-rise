#include "Game.h"
#include "Update.h"

double Game::deltaTime = 0;

int Game::playGame()
{
	double currentFrame = 0.0f;
	double lastFrame = 0.0f;
	while (!m_window.windowClosed())
	{
		// update deltatime
		currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame; glm::clamp(deltaTime, 0.0, 5.);

		Update::update();
		// render as often as possible
		m_window.render();
	}
	return 0;
}
