#pragma once

#include "ScriptObject.h"

class DestroyAfterTime : public ScriptObject
{
public:
	void Start()override;
	void Update()override;


	float maxTimeAlive = 1.0f;
private:
	float timeAliveLeft = 1.0f;
};

