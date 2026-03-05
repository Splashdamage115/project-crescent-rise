#pragma once

#include "ScriptObject.h"

class EnemyMovement : public ScriptObject
{
public:
	void Start() override;
	void Update() override;
private:
	bool canSeePlayer = false;
	float m_moveSpeed = 5.0f;
};

