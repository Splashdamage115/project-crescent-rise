#include "GameLoop.h"

int GameLoop::playGame()
{
	while (!m_window.windowClosed())
	{
		m_window.render();
	}
	return 0;
}
