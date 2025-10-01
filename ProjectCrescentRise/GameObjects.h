#pragma once

#include "GameObject.h"

class GameObjects
{
public:
	static void addNewObjectToPool(const GameObject& t_newObject);
private:
	static std::vector<GameObject> m_gameObjects;
};

