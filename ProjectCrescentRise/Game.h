#pragma once

#include "Library.h"
#include "GameObjects.h"
#include "KeyScan.h"
#include "Model.h"
#include "SurfaceGrass.h"

class Game
{
public:
	void initGame();

	int playGame();

	void initFloor();

	static double deltaTime;	
private:
	std::shared_ptr<mouseKeyInput> mski;
	std::shared_ptr<GameObject> PlanetObj;
	std::shared_ptr<GameObject> waterObj;
	std::shared_ptr<GameObject> camObj;
	std::shared_ptr<GameObject> crateCube;
	std::shared_ptr<GameObject> skyBox;
	std::shared_ptr<GameObject> guiObject;
	std::shared_ptr<GameObject> grass;

	std::shared_ptr<GameObject> Enemy;

	std::shared_ptr <Model> treeModel;
	std::shared_ptr <Model> rockModel;
	std::shared_ptr<SurfaceGrass> grassModel;
};

