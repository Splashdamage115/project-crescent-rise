#pragma once

#include "Library.h"
#include "GameObjects.h"
#include "KeyScan.h"

class Game
{
public:
	void initGame();

	int playGame();

	void initFloor();

	static double deltaTime;	
private:
	std::shared_ptr<mouseKeyInput> mski;
	std::shared_ptr<GameObject> floorObj;
	std::shared_ptr<GameObject> camObj;
	std::shared_ptr<GameObject> waterObj;
};

