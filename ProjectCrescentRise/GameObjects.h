#pragma once

#include "GameObject.h"

class GameObjects
{
public:
	static void addNewObjectToPool(std::shared_ptr < GameObject> t_newObject);
	static void renderAll();
private:
	static std::vector<std::shared_ptr<GameObject>> m_gameObjects;
};

