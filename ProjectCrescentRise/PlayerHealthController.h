#pragma once

#include "HealthController.h"

class PlayerHealthController : public HealthController
{
public:
	void Start()override;
	void Update()override;
	void expire()override;
	void secondaryHitEffect()override;

	void EnableGodMode(std::string _);
private:
	std::shared_ptr<GameObject> screenOverlay;
	bool dead = false;
	bool godMode = false;
};

