#pragma once

#include "ScriptObject.h"

class Mover : public ScriptObject
{
public:
	void Update() override;

	glm::vec3 velocity = glm::vec3(0.0f);
private:

};
