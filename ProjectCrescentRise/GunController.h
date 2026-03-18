#pragma once

#include "ScriptObject.h"
#include "Model.h"

class Particle;

enum class WeaponType
{
	ShotGun, AK
};

struct weaponInfo
{
	WeaponType weaponType;
	std::shared_ptr<Model> gunModel;

	int maxMagAmmo = 6;
	int reserveAmmo = 24;

	float fireRate = 0.5f;
	float reloadTime = 2.0f;

	float shotDamage = 100.0f;

	bool fullAuto = false;
};

class GunController : public ScriptObject
{
public:
	void Start() override;
	void Update() override;
	virtual void shootWeapon();
	virtual void leftClickDown();
	virtual void leftClickUp();


	virtual void reloadWeapon();
	void setGunModel(std::shared_ptr<Model> model);

	void swapModel();

	std::shared_ptr<Model> gunModel;

	Transform muzzleOffset;
protected:
	void setCurrentWeapon(WeaponType t_newType);
	void initialiseWeapons();
	void refillMagazine();
	void handleHitScan();
	void setWeaponInfo(weaponInfo& t_weaponInfo);

	std::shared_ptr<Particle> shootParticle;

	int magAmmo = 6;
	int maxMagAmmo = 6;
	int reserveAmmo = 24;

	float fireRate = 0.5f;
	float reloadTime = 2.0f;
	float timeSinceLastShot = 0.0f;
	float timeSinceReloadStart = 0.0f;

	bool reloading = false;

	float shotDamage = 100.0f;

	bool fullAuto = false;
	bool shooting = false;

	int currentlySelectedObj = 0;

	WeaponType m_weaponType = WeaponType::AK;

	std::vector<weaponInfo> m_gunVariants;
};

