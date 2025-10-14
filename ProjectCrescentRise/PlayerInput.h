#pragma once

#include "ScriptObject.h"

class PlayerInput : public ScriptObject
{
public:
	void Update();
private:
	double lastMouseX = 0.;
	double lastMouseY = 0.;
};

