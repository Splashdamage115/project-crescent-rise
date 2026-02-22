#include "ScriptObject.h"
#include "GameObject.h"

void ScriptObject::setTransform(std::shared_ptr<Transform> t_transform)
{
	transform = t_transform;
}

void ScriptObject::setParentHighlight(std::shared_ptr<bool> t_highlight)
{
	parentHighlight = t_highlight;
}
