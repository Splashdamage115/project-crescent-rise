#pragma once

#include "ScriptObject.h"

class PlayerInput : public ScriptObject
{
public:
	void Update();
private:
	double lastMouseX = 0.;
	double lastMouseY = 0.;


	float currentSpeed = 10.0f;
	const float speedIncrease = 5.0f;
	const float minSpeed = 10.0f;
	const float maxSpeed = 300.0f;
};

