#pragma once

#include "ScriptObject.h"

class PlayerInput : public ScriptObject
{
public:
	void Start() override;
	void Update() override;

	static glm::vec3 playerPosition;
	static bool noClipEnabled;

	void handleInteractRelease();
private:

	double lastMouseX = 0.;
	double lastMouseY = 0.;


	float currentSpeed = 10.0f;
	const float speedIncrease = 5.0f;
	const float minSpeed = 12.0f;
	const float maxSpeed = 20.0f;

	std::shared_ptr<GameObject> highlightedObj;
};

