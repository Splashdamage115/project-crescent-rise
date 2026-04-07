#pragma once

#include "ScriptObject.h"

class HealthController : public ScriptObject
{
public:
	void Start() override;
	void Update() override;
	virtual void sendMessage(const std::string& t_messageString, std::any amount)override;
	
	float invincibilityTime = 3.0f;
protected:
	virtual void secondaryHitEffect();
	virtual void changeHealth(float amount);
	virtual void expire();

	float currentHealth = 100.f;
	float maxHealth = 100.f;

};

