#pragma once

#include "Library.h"
#include <any>

class GameObject;

class ScriptObject
{
public:
	enum class RenderPriority
	{
		GUI, Cull, noCull, Viewmodel
	};

	RenderPriority renderPriority = RenderPriority::noCull;

	void setTransform(std::shared_ptr<Transform> t_transform);
	void setParentHighlight(std::shared_ptr<bool> t_parent);
	std::shared_ptr<Transform> getTransform() { return transform; }

	virtual void Start() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void sendMessage(const std::string& t_messageType) {}
	virtual void sendMessage(const std::string& t_messageType, std::any t_messageParameters) {}
protected:
	std::shared_ptr<bool> parentHighlight;
	std::shared_ptr<Transform> transform;
	bool enabled = true;
};

