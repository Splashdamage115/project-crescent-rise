#include "GameObject.h"
#include "Update.h"
#include "PlayerInput.h"

GameObject::GameObject()
{
	transform = std::make_shared<Transform>();
	highlighted = std::make_shared<bool>();
	active = true;
}

void GameObject::move(const glm::vec3& delta)
{
	transform->position += delta;
}


bool GameObject::addScript(std::shared_ptr<ScriptObject> t_newScript)
{
	try {
		// TO DO: fix this potential circular include of self stopping self destruction
		t_newScript->setTransform(transform);
		t_newScript->setParentHighlight(highlighted);

		t_newScript->Start();

		m_scripts.push_back(t_newScript);

		Update::append([t_newScript]() { t_newScript->Update(); });
	}
	catch (std::exception)
	{
		return false;
	}
	return true;
}

void GameObject::callRenders(ScriptObject::RenderPriority t_currentPriority)
{
	for (unsigned int i = 0; i < m_scripts.size(); i++)
	{
		if (m_scripts.at(i)->renderPriority == t_currentPriority)
		{
			if (t_currentPriority == ScriptObject::RenderPriority::Cull)
			{
				
				if (glm::distance(transform->position, PlayerInput::playerPosition) < 30.f)
				{
					m_scripts.at(i)->setTransform(transform);
					m_scripts.at(i)->Render();
				}
			}
			else
			{
				m_scripts.at(i)->Render();
			}
		}
	}
}

void GameObject::sendMessage(const std::string& t_messageType)
{
	for (auto& i : m_scripts)
	{
		i->sendMessage(t_messageType);
	}
}

void GameObject::sendMessage(const std::string& t_messageType, std::any t_messageParameters)
{
	for (auto& i : m_scripts)
	{
		i->sendMessage(t_messageType, t_messageParameters);
	}
}

