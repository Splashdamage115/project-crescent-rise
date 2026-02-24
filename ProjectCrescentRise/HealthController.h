#pragma once

#include "ScriptObject.h"

class HealthController : public ScriptObject
{
public:
	void Start() override;
	void Update() override;
	void sendMessage(const std::string& t_messageString, std::any amount)override;

private:
	void changeHealth(float amount);
	void expire();

	float currentHealth = 100.f;
	float maxHealth = 100.f;
};

