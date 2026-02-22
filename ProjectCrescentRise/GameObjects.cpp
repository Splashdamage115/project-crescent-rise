#include "GameObjects.h"
#include "ScriptObject.h"
#include "PlayerInput.h"

std::vector<std::shared_ptr<GameObject>> GameObjects::m_gameObjects;
std::mutex GameObjects::gameObjectsMutex;
float GameObjects::tagDetectDistance = 30.0f;

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


std::vector<std::shared_ptr<GameObject>> GameObjects::getAllOfTag(const std::string& t_tag)
{
	std::vector<std::shared_ptr<GameObject>> go;
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (glm::distance(m_gameObjects.at(i)->transform->position, PlayerInput::playerPosition) < tagDetectDistance)
		{
			bool foundObj = false;
			for (int l = 0; l < m_gameObjects.at(i)->tags.size(); l++)
			{
				if (m_gameObjects.at(i)->tags.at(l) == t_tag)
				{
					go.push_back(m_gameObjects.at(i));
					foundObj = true;
					break;
				}
			}
			if (foundObj) continue;
		}
	}
	return go;
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
	// render viewmodel on top of world geometry
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i) != nullptr)
		{
			if (m_gameObjects.at(i)->active)
			{
				m_gameObjects.at(i)->callRenders(ScriptObject::RenderPriority::Viewmodel);
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
