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

	virtual void sendMessage(const std::string& t_messageType);
	virtual void sendMessage(const std::string& t_messageType, std::any t_messageParameters);

	std::shared_ptr<Transform> transform;

	bool active = false;
	bool hidden = false;
	std::shared_ptr<bool> highlighted;

	std::vector<std::string> tags;
protected:
	std::vector<std::shared_ptr<ScriptObject>> m_scripts;
private:

};

