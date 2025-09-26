#pragma once

#include "Library.h"
#include "Window.h"

class Game
{
public:
	int playGame();

	static double deltaTime;
private:
	Window m_window;
};

