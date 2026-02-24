#include "ScriptObject.h"
#include "GameObject.h"

ScriptObject::~ScriptObject()
{
	parent = nullptr;
}

void ScriptObject::setTransform(std::shared_ptr<Transform> t_transform)
{
	transform = t_transform;
}

void ScriptObject::setParent(GameObject* t_parent)
{
	parent = t_parent;
}
