#pragma once

#include "Library.h"

class ScriptObject;

class GameObject
{
public:
	GameObject();

	void move(const glm::vec3& delta);
	bool addScript(std::shared_ptr<ScriptObject> t_newScript);

	Transform transform{};

	bool active = false;
	bool hidden = false;
protected:
	std::vector<std::shared_ptr<ScriptObject>> m_scripts;
private:

};

