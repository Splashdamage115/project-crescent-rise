#pragma once

#include "GameObject.h"
#include <mutex>

class GameObjects
{
public:
	static void addNewObjectToPool(std::shared_ptr < GameObject> t_newObject);
	static std::vector<std::shared_ptr<GameObject>> getAllOfTag(const std::string& t_tag, bool visibleOnly = false);
	static void renderAll();
	static const std::vector<std::shared_ptr<GameObject>>& getAllObjects() { return m_gameObjects; }
private:
	static std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	static std::mutex gameObjectsMutex;
	static float tagDetectDistance;
};
