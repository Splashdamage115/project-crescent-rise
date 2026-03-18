#include "GunController.h"
#include "Game.h"
#include "Raycast.h"
#include "GameObjects.h"
#include "ParticleController.h"
#include "Window.h"

void GunController::Start()
{
	initialiseWeapons();
	currentlySelectedObj = -1;
	swapModel();
	//std::shared_ptr<Model> m = std::make_shared<Model>();
	//
	//switch (m_weaponType)
	//{
	//case WeaponType::ShotGun:
	//{
	//	m->modelOffset.scale = { 1.f, 1.f, 1.f };
	//	m->modelOffset.rotation = { 90.0f, 180.0f, 180.0f };
	//	m->modelOffset.position = { 0.5f, -0.3f, -1.0f };
	//	m->loadLocation = "./Assets/Mesh/gun.fbx";
	//	m->colour = glm::vec3(1.f, 1.f, 1.f);
	//	m->followCam = true;
	//
	//	//gunModel->useOffsetMover = true;
	//	m->rotation = glm::vec3(0.f, 90.f, 0.f);
	//	m->textureLoc2 = "./Assets/Images/metal.jpg";
	//	m->textureLoc1 = "./Assets/Images/wood.jpg";
	//
	//	fireRate = 0.7f;
	//	magAmmo = 6;
	//	maxMagAmmo = 6;
	//	reserveAmmo = 24;
	//}
	//	break;
	//case WeaponType::AK:
	//{
	//	m->modelOffset.scale = { 1.f, 1.f, 1.f };
	//	m->modelOffset.rotation = { 90.f, 0.f, 180.f };
	//	m->modelOffset.position = { 0.5f, -0.3f, -1.0f };
	//	m->loadLocation = "./Assets/Mesh/gunAK.fbx";
	//	m->colour = glm::vec3(1.f, 1.f, 1.f);
	//	m->followCam = true;
	//
	//	//gunModel->useOffsetMover = true;
	//	m->rotation = glm::vec3(0.f, 90.f, 0.f);
	//	m->textureLoc1 = "./Assets/Images/metal.jpg";
	//	m->textureLoc2 = "./Assets/Images/wood.jpg";
	//
	//	fireRate = 0.2f;
	//	magAmmo = 30;
	//	maxMagAmmo = 30;
	//	reserveAmmo = 120;
	//	shotDamage = 40;
	//	fullAuto = true;
	//}
	//	break;
	//default:
	//	break;
	//} 
	//
	//
	//*gunModel = *m;
	//
	//gunModel->Start();



	std::shared_ptr<keyboardInput> ki = std::make_shared<keyboardInput>();
	ki->active = true;
	ki->keyCode = KeyScan::KeyCode::R;
	ki->function = ([this]() { this->reloadWeapon(); });
	KeyScan::append(ki, true);

	std::shared_ptr<mouseKeyInput> mski = std::make_shared<mouseKeyInput>();
	mski->active = true;
	mski->keyCode = KeyScan::MouseKeyCode::LeftMouse;
	mski->function = ([this]() { this->leftClickUp(); });
	KeyScan::append(mski, true);

	std::shared_ptr<mouseKeyInput> umski = std::make_shared<mouseKeyInput>();
	umski->active = true;
	umski->keyCode = KeyScan::MouseKeyCode::LeftMouse;
	umski->function = ([this]() { this->leftClickDown(); });
	KeyScan::append(umski, false);

	std::shared_ptr<mouseKeyInput> mmski = std::make_shared<mouseKeyInput>();
	mmski->active = true;
	mmski->keyCode = KeyScan::MouseKeyCode::MiddleMouse;
	mmski->function = ([this]() { this->swapModel(); });
	KeyScan::append(mmski, true);
}

void GunController::Update()
{
	if(timeSinceLastShot > 0.f) timeSinceLastShot -= static_cast<float>(Game::deltaTime);
	if (reloading)
		if (timeSinceReloadStart > 0.f) timeSinceReloadStart -= static_cast<float>(Game::deltaTime);
		else
		{
			reloading = false;
			refillMagazine();
		}
	
	if (fullAuto && shooting) shootWeapon();
}

void GunController::leftClickDown()
{
	if (fullAuto)
	{
		shooting = true;
	}
}

void GunController::leftClickUp()
{
	if (fullAuto)
	{
		shooting = false;
	}
	else
	{
		shootWeapon();
	}
}

void GunController::shootWeapon()
{
	if (magAmmo > 0 && timeSinceLastShot <= 0.f)
	{
		std::cout << "Pew Pew!\n";

		glm::vec3 gunCameraSpace = gunModel->modelOffset.position + muzzleOffset.position;
		glm::mat4 invView = glm::inverse(Window::Get().GetView());
		glm::vec4 worldPosition = invView * glm::vec4(gunCameraSpace, 1.0f);
		glm::vec3 particlePos = glm::vec3(worldPosition);
		Transform positionOverride;
		positionOverride.position = particlePos;
		positionOverride.rotation = glm::vec3(0.f, 0.f, 0.f);
		positionOverride.scale = glm::vec3(0.5f);
		shootParticle = ParticleController::SpawnNewParticle("./Assets/Images/Particles/basicParticle.png", positionOverride, 0.2f, glm::vec2(9,9), glm::vec2(835.f, 796.f));
		
		
		//std::shared_ptr<Particle> muzzleFlash = std::make_shared<Particle>();
		//int chosen = rand() % 2;
		//
		//switch (chosen)
		//{
		//case 0:
		//	muzzleFlash->textureLocation = "./Assets/Images/Particles/basicParticle.png";
		//	muzzleFlash->frameAmt = glm::vec2(9, 9);
		//	muzzleFlash->textureSize = glm::vec2(835.f, 796.f);
		//	break;
		//case 1:
		//	muzzleFlash->textureLocation = "./Assets/Images/Particles/explosion1.png";
		//	muzzleFlash->frameAmt = glm::vec2(10, 5);
		//	muzzleFlash->textureSize = glm::vec2(1000.f, 500.f);
		//	break;
		//case 2:
		//	muzzleFlash->textureLocation = "./Assets/Images/Particles/basicParticle.png";
		//	muzzleFlash->frameAmt = glm::vec2(9, 9);
		//	muzzleFlash->textureSize = glm::vec2(835.f, 796.f);
		//	break;
		//default:
		//	break;
		//}






		magAmmo--;
		timeSinceLastShot = fireRate;
		if(gunModel) gunModel->playAnimation("Shoot");
		handleHitScan();
		
	}
	else if (magAmmo == 0)
	{
		reloading = true;
		refillMagazine();
	}
}

void GunController::reloadWeapon()
{
	reloading = true;
	if(magAmmo != maxMagAmmo)
		refillMagazine();
}

void GunController::setGunModel(std::shared_ptr<Model> model)
{ 
	gunModel = model;

	muzzleOffset.position = glm::vec3(-0.2f, 0.1f, -0.5f);
}

void GunController::swapModel()
{
	currentlySelectedObj++;
	if (m_gunVariants.size() <= currentlySelectedObj) currentlySelectedObj = 0;

	setCurrentWeapon(m_gunVariants.at(currentlySelectedObj).weaponType);
}

void GunController::setCurrentWeapon(WeaponType t_newType)
{
	bool found = false;
	for (auto i : m_gunVariants)
	{
		if (i.weaponType == t_newType)
		{
			found = true;
			setWeaponInfo(i);
			continue;
		}
		else
		{
			continue;
		}
	}
	if (!found)
	{
		setWeaponInfo(m_gunVariants.at(0));
	}
}

void GunController::initialiseWeapons()
{
	{
		weaponInfo newWeapon;
		std::shared_ptr<Model> m = std::make_shared<Model>();

		m->modelOffset.scale = { 1.f, 1.f, 1.f };
		m->modelOffset.rotation = { 90.0f, 180.0f, 180.0f };
		m->modelOffset.position = { 0.5f, -0.3f, -1.0f };
		m->loadLocation = "./Assets/Mesh/gun.fbx";
		m->colour = glm::vec3(1.f, 1.f, 1.f);
		m->followCam = true;
		//gunModel->useOffsetMover = true;
		m->rotation = glm::vec3(0.f, 90.f, 0.f);
		m->textureLoc2 = "./Assets/Images/metal.jpg";
		m->textureLoc1 = "./Assets/Images/wood.jpg";

		newWeapon.gunModel = m;

		newWeapon.fireRate = 0.7f;
		newWeapon.reloadTime = 4.0f;
		newWeapon.fullAuto = false;
		newWeapon.shotDamage = 80;
		newWeapon.weaponType = WeaponType::ShotGun;
		newWeapon.maxMagAmmo = 6;
		newWeapon.reserveAmmo = 24;

		m_gunVariants.push_back(newWeapon);
	}

	{
		weaponInfo newWeapon;
		std::shared_ptr<Model> m = std::make_shared<Model>();

		m->modelOffset.scale = { 1.f, 1.f, 1.f };
		m->modelOffset.rotation = { 90.f, 0.f, 180.f };
		m->modelOffset.position = { 0.5f, -0.3f, -1.0f };
		m->loadLocation = "./Assets/Mesh/gunAK.fbx";
		m->colour = glm::vec3(1.f, 1.f, 1.f);
		m->followCam = true;

		//gunModel->useOffsetMover = true;
		m->rotation = glm::vec3(0.f, 90.f, 0.f);
		m->textureLoc1 = "./Assets/Images/metal.jpg";
		m->textureLoc2 = "./Assets/Images/wood.jpg";

		newWeapon.gunModel = m;

		newWeapon.fireRate = 0.2f;
		newWeapon.reloadTime = 2.0f;
		newWeapon.fullAuto = true;
		newWeapon.shotDamage = 40;
		newWeapon.weaponType = WeaponType::AK;
		newWeapon.maxMagAmmo = 30;
		newWeapon.reserveAmmo = 120;

		m_gunVariants.push_back(newWeapon);
	}
}

void GunController::refillMagazine()
{
	if (reserveAmmo > 0 && timeSinceReloadStart <= 0.f && !reloading)
	{
		if (gunModel) gunModel->playAnimation("Reload");
		std::cout << "Reloading...\n";
		int ammoToReload = std::min(reserveAmmo, maxMagAmmo - magAmmo);
		magAmmo += ammoToReload;
		reserveAmmo -= ammoToReload;
		timeSinceReloadStart = reloadTime;
	}
}

void GunController::handleHitScan()
{
	Ray ray = Ray::FromCamera();

	std::vector<std::shared_ptr<GameObject>> go = GameObjects::getAllOfTag("shootable");

	float hitDist = 999999.0f;
	float closestHit = 99999999.0f;
	int closestNum = -1;
	for (int i = 0; i < go.size(); i++)
	{
		glm::vec3 enemyPos = go.at(i)->transform->position;
		float enemyRadius = 1.0f;
		if (ray.hitsphere(enemyPos, enemyRadius, hitDist))
		{
			if (closestHit > hitDist)
			{
				closestHit = hitDist;
				closestNum = i;
			}
		}
	}
	if (closestNum != -1)
	{
		go.at(closestNum)->sendMessage("DAMAGE", shotDamage);
		std::cout << "Hit enemy for " << shotDamage << " damage!\n";
	}
}

void GunController::setWeaponInfo(weaponInfo& t_weaponInfo)
{
	shotDamage = t_weaponInfo.shotDamage;
	fireRate = t_weaponInfo.fireRate;
	maxMagAmmo = t_weaponInfo.maxMagAmmo;
	reserveAmmo = t_weaponInfo.reserveAmmo;
	timeSinceReloadStart = 0.f;
	refillMagazine();

	reloadTime = t_weaponInfo.reloadTime;

	fullAuto = t_weaponInfo.fullAuto;

	*gunModel = *t_weaponInfo.gunModel;

	gunModel->Start();
}
