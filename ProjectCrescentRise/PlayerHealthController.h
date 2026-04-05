#pragma once

#include "HealthController.h"

class PlayerHealthController : public HealthController
{
public:
	void Start()override;
	void Update()override;
	void expire()override;

	void EnableGodMode(std::string _);
private:
	bool dead = false;
	bool godMode = false;
};

