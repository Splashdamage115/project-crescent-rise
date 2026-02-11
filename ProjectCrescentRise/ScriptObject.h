#pragma once

#include "Library.h"

class ScriptObject
{
public:
	enum class RenderPriority
	{
		GUI, Cull, noCull
	};

	RenderPriority renderPriority = RenderPriority::noCull;

	void setTransform(std::shared_ptr<Transform> t_transform);
	std::shared_ptr<Transform> getTransform() { return transform; }

	virtual void Start() {}
	virtual void Update() {}
	virtual void Render() {}
protected:
	std::shared_ptr<Transform> transform;
	bool enabled = true;
};

