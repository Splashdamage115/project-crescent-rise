#include "GameObjects.h"
#include "ScriptObject.h"

std::vector<std::shared_ptr<GameObject>> GameObjects::m_gameObjects;
std::mutex GameObjects::gameObjectsMutex;

void GameObjects::addNewObjectToPool(std::shared_ptr< GameObject> t_newObject)
{
	std::lock_guard<std::mutex> lock(gameObjectsMutex);
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i) != nullptr)
		{
			if (!m_gameObjects.at(i)->active)
			{
				m_gameObjects.at(i) = t_newObject;
				return;
			}
		}
	}
	m_gameObjects.push_back(t_newObject);
}

void GameObjects::renderAll()
{
	std::lock_guard<std::mutex> lock(gameObjectsMutex);
	// render normals
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i) != nullptr)
		{
			if (m_gameObjects.at(i)->active)
			{
				m_gameObjects.at(i)->callRenders(ScriptObject::RenderPriority::noCull);
				m_gameObjects.at(i)->callRenders(ScriptObject::RenderPriority::Cull);
			}
		}
	}
	// render GUI
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i) != nullptr)
		{
			if (m_gameObjects.at(i)->active)
			{
				m_gameObjects.at(i)->callRenders(ScriptObject::RenderPriority::GUI);
			}
		}
	}
}
