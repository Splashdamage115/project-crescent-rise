#pragma once

#include "ScriptObject.h"

class OnlineMover : public ScriptObject
{
public:
	std::atomic<glm::vec3> position;
	void Update() override;
private:

};

