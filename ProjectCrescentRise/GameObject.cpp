#include "GameObject.h"
#include "Update.h"
#include "ScriptObject.h"

GameObject::GameObject()
{
	active = true;
}

void GameObject::move(const glm::vec3& delta)
{
	transform.position += delta;
}

bool GameObject::addScript(std::shared_ptr<ScriptObject> t_newScript)
{
	try {
		// TO DO: fix this potential circular include of self stopping self destruction
		t_newScript->setTransform(std::make_shared<Transform>(transform));

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
