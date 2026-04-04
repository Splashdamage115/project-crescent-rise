#pragma once

#include "HealthController.h"

class PlayerHealthController : public HealthController
{
public:
	void Start()override;
	void Update()override;
	void expire()override;

private:
	bool dead = false;
};

