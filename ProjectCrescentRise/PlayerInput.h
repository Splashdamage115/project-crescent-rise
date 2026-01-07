#pragma once

#include "ScriptObject.h"

class PlayerInput : public ScriptObject
{
public:
	void Start() override;
	void Update() override;

	static glm::vec3 playerPosition;
	static bool noClipEnabled;
private:
	double lastMouseX = 0.;
	double lastMouseY = 0.;


	float currentSpeed = 10.0f;
	const float speedIncrease = 50.0f;
	const float minSpeed = 10.0f;
	const float maxSpeed = 300.0f;
};

