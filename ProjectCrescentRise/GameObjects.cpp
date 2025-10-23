#include "GameObjects.h"

std::vector<std::shared_ptr<GameObject>> GameObjects::m_gameObjects;

void GameObjects::addNewObjectToPool(std::shared_ptr< GameObject> t_newObject)
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i) == nullptr)
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
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects.at(i)->active)
		{
			m_gameObjects.at(i)->callRenders();
		}
	}
}
