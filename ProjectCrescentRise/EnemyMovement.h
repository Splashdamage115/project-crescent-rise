#pragma once

#include "ScriptObject.h"

class EnemyMovement : public ScriptObject
{
public:
	void Start() override;
	void Update() override;

	bool moving = true;
private:
	bool canSeePlayer = false;
	float m_moveSpeed = 5.0f;
};

