#pragma once

#include "ScriptObject.h"

class ApplyDamageRadius : public ScriptObject
{
public:
	void Start();
	void Update();

	std::shared_ptr<GameObject> targetObject;
	float DamageRadius = 0.2f;
	float damageAmt = 5.0f;
private:
};

