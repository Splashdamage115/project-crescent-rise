#pragma once

#include "Library.h"

class ScriptObject
{
public:
	enum class RenderPriority
	{
		GUI, Normal
	};

	RenderPriority renderPriority = RenderPriority::Normal;

	void setTransform(std::shared_ptr<Transform> t_transform);

	virtual void Start() {}
	virtual void Update() {}
	virtual void Render() {}
protected:
	std::shared_ptr<Transform> transform;
	bool enabled = true;
};

