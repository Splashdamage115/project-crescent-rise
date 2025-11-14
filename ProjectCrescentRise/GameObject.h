#pragma once

#include "Library.h"
#include "ScriptObject.h"

class GameObject
{
public:
	GameObject();

	void move(const glm::vec3& delta);
	bool removeScript(std::shared_ptr<ScriptObject> t_script);
	bool addScript(std::shared_ptr<ScriptObject> t_newScript);
	void callRenders(ScriptObject::RenderPriority t_currentPriority);

	std::shared_ptr<Transform> transform;

	bool active = false;
	bool hidden = false;
protected:
	std::vector<std::shared_ptr<ScriptObject>> m_scripts;
private:

};

