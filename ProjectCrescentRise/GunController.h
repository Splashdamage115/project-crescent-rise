#pragma once

#include "ScriptObject.h"
#include "Model.h"

class GunController : public ScriptObject
{
public:
	void Start() override;
	void Update() override;
	virtual void shootWeapon();
	virtual void reloadWeapon();
	void setGunModel(std::shared_ptr<Model> model);

	std::shared_ptr<Model> gunModel;
protected:
	void refillMagazine();
	void handleHitScan();

	int magAmmo = 6;
	int maxMagAmmo = 6;
	int reserveAmmo = 24;

	float fireRate = 0.5f;
	float reloadTime = 2.0f;
	float timeSinceLastShot = 0.0f;
	float timeSinceReloadStart = 0.0f;

	float shotDamage = 10.0f;
};

