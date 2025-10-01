#pragma once

#include "Library.h"
#include "Window.h"

class Game
{
public:
	void initGame();

	int playGame();

	static double deltaTime;	
private:
	Window m_window;
};

