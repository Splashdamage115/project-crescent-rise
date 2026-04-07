#include "ScriptObject.h"
#include "GameObject.h"

ScriptObject::~ScriptObject()
{
	parent.lock() = nullptr;
}

void ScriptObject::setTransform(std::shared_ptr<Transform> t_transform)
{
	transform = t_transform;
}

void ScriptObject::setParent(std::shared_ptr<GameObject> t_parent)
{
	 parent = t_parent;
}
