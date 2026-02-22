#pragma once

#include "GameObject.h"
#include <mutex>

class GameObjects
{
public:
	static void addNewObjectToPool(std::shared_ptr < GameObject> t_newObject);
	static std::vector<std::shared_ptr<GameObject>> getAllOfTag(const std::string& t_tag);
	static void renderAll();
private:
	static std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	static std::mutex gameObjectsMutex;
	static float tagDetectDistance;
};
