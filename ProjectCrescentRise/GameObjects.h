#pragma once

#include "GameObject.h"

class GameObjects
{
public:
	static void addNewObjectToPool(const GameObject& t_newObject);
	static void renderAll();
private:
	static std::vector<GameObject> m_gameObjects;
};

