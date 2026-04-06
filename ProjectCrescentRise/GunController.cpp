#include "GunController.h"
#include "Game.h"
#include "Raycast.h"
#include "GameObjects.h"
#include "ParticleController.h"
#include "Window.h"

void GunController::Start()
{
	m_gunVariants.clear();
	initialiseWeapons();
	currentlySelectedObj = -1;
	swapModel();

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

	if (recoilMoveTimeLeft >= 0.f)
	{
		recoilMoveTimeLeft -= static_cast<float>(Game::deltaTime);

		transform->rotation.y -= xDelta * (Game::deltaTime);
		transform->rotation.x -= yDelta * (Game::deltaTime);
	}

	if (recoilTimeLeft > 0.f)
	{
		recoilTimeLeft -= static_cast<float>(Game::deltaTime);
		if (recoilUpTime > 0.f)
		{
			recoilUpTime -= static_cast<float>(Game::deltaTime);
			float t = 1.0f - (recoilUpTime / 0.1f);				
			int flip = 1;
			if (flipXRot) flip = -1;
			gunModel->modelOffset.rotation.x = glm::mix(originalXRot, originalXRot + ((recoilStrength / 2.f) * flip), t);
			gunModel->modelOffset.position.y = glm::mix(originalPosition, originalPosition + 0.03f, t);
			gunModel->modelOffset.position.x = glm::mix(originalPositionx, originalPositionx + 0.07f, t);
			gunModel->modelOffset.rotation.y = glm::mix(originalYRot, originalYRot + 1.f, t);
		}
		else
		{
			float t = 1.0f - (tiltBackTimeLeft / 0.1f);
			int flip = 1;
			if (flipXRot) flip = -1;
			gunModel->modelOffset.rotation.x = glm::mix(originalXRot + ((recoilStrength / 2.f) * flip), originalXRot, t);
			gunModel->modelOffset.position.y = glm::mix(originalPosition + 0.03f, originalPosition, t);
			gunModel->modelOffset.position.x = glm::mix(originalPositionx + 0.07f, originalPositionx, t);
			gunModel->modelOffset.rotation.y = glm::mix(originalYRot + 1.f, originalYRot, t);
		}
	}

	if (reloading)
		if (timeSinceReloadStart > 0.f)
		{
			if (timeSinceReloadStart < 0.5f && tiltBackTimeLeft <= 0.f) 
			{
				tiltBackTimeLeft = 0.5f;
			}
			if (tiltBackTimeLeft >= 0.f)
			{
				tiltBackTimeLeft -= Game::deltaTime;
				if (tiltDownTimeLeft < 0.0f) tiltDownTimeLeft = 0.0f;

				float t = 1.0f - (tiltBackTimeLeft / 0.5f);
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;

				int flip = 1;
				if (flipXRot) flip = -1;
				
				gunModel->modelOffset.rotation.x = glm::mix(originalXRot - (45.0f * flip), originalXRot, t);
				gunModel->modelOffset.position.y = glm::mix(originalPosition - 0.3f, originalPosition, t);
				gunModel->modelOffset.rotation.y = glm::mix(originalYRot - 20.f, originalYRot, t);
			}

			if (tiltDownTimeLeft > 0.0f)
			{
				tiltDownTimeLeft -= Game::deltaTime;
				if (tiltDownTimeLeft < 0.0f) tiltDownTimeLeft = 0.0f;

				float t = 1.0f - (tiltDownTimeLeft / 0.5f);
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;

				int flip = 1;
				if (flipXRot) flip = -1;

				gunModel->modelOffset.rotation.x = glm::mix(originalXRot, originalXRot - (45.0f * flip), t);
				gunModel->modelOffset.position.y = glm::mix(originalPosition, originalPosition - 0.3f, t);
				gunModel->modelOffset.rotation.y = glm::mix(originalYRot, originalYRot - 20.f, t);
			}

			//gunModel->modelOffset.rotation.x -= 45.f * Game::deltaTime;


			timeSinceReloadStart -= static_cast<float>(Game::deltaTime);
		}
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
	if (magAmmo > 0 && timeSinceLastShot <= 0.f && !reloading)
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

		//ParticleController::SpawnNewParticle("./Assets/Images/Particles/smoke1.png", positionOverride, 0.1f, glm::vec2(1, 1), glm::vec2(512.f, 512.f));
		//ParticleController::SpawnNewParticle("./Assets/Images/Particles/smoke2.png", positionOverride, 0.1f, glm::vec2(1, 1), glm::vec2(512.f, 512.f));

		int chosen = rand() % 2;
		int chosenFlare = (rand() % 6) + 1;
		std::string loc = "./Assets/Images/Particles/flare_";
		loc += std::to_string(chosenFlare);
		loc += ".png";

		if (m_weaponType == WeaponType::glock)
		{
			positionOverride.position += glm::vec3(0.f, 0.f, -0.2f);
		}

		auto p = ParticleController::SpawnNewParticle(loc, positionOverride, 0.1f, glm::vec2(1, 1), glm::vec2(512.f, 512.f));

		p->positionOverride->scale = glm::vec3(1.8f);

		positionOverride.position += glm::vec3(0.f, 0.f, 0.05f);
		if (chosen == 0)
		{
			shootParticle = ParticleController::SpawnNewParticle("./Assets/Images/Particles/basicParticle.png", positionOverride, 0.2f, glm::vec2(9, 9), glm::vec2(835.f, 796.f));
			//ParticleController::SpawnNewParticle("./Assets/Images/Particles/smoke2.png", positionOverride, 0.3f, glm::vec2(1, 1), glm::vec2(512.f, 512.f));
		}
		else if (chosen == 1)
		{
			shootParticle = ParticleController::SpawnNewParticle("./Assets/Images/Particles/explosion1.png", positionOverride, 0.3f, glm::vec2(10, 3), glm::vec2(1000.f, 277.f), 0.01f);
			//ParticleController::SpawnNewParticle("./Assets/Images/Particles/smoke1.png", positionOverride, 0.4f, glm::vec2(1, 1), glm::vec2(512.f, 512.f));
		}
		shootParticle->positionOverride->scale = glm::vec3(0.25f);

		recoilTimeLeft = 0.2f;
		recoilUpTime = 0.1f;

		xDelta = static_cast<float>((rand() % recoilStrength) - ((recoilStrength / 2) - 1));
		yDelta = static_cast<float>(recoilStrength);

		recoilMoveTimeLeft = 0.125f;

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
		timeSinceReloadStart = reloadTime;
		tiltDownTimeLeft = 0.5f;
		refillMagazine();
	}
}

void GunController::reloadWeapon()
{
	reloading = true;
	timeSinceReloadStart = reloadTime;
	tiltDownTimeLeft = 0.5f;

	if(magAmmo != maxMagAmmo)
		refillMagazine();
}

void GunController::setGunModel(std::shared_ptr<Model> model)
{ 
	gunModel = model;

	muzzleOffset.position = glm::vec3(-0.18f, 0.2f, -0.5f);

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

		newWeapon.recoilStrength = 25;
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

		m->modelOffset.scale = { 0.023f, 0.023f, 0.023f };
		m->modelOffset.rotation = { 90.0f, 180.0f, 180.0f };
		m->modelOffset.position = { 0.5f, -0.3f, -1.0f };
		m->loadLocation = "./Assets/Mesh/glock.fbx";
		m->colour = glm::vec3(1.f, 1.f, 1.f);
		m->followCam = true;
		//gunModel->useOffsetMover = true;
		m->rotation = glm::vec3(0.f, 90.f, 0.f);
		m->textureLoc1 = "./Assets/Images/glockColour.png";

		newWeapon.gunModel = m;

		newWeapon.recoilStrength = 10;
		newWeapon.fireRate = 0.f;
		newWeapon.reloadTime = 2.0f;
		newWeapon.fullAuto = false;
		newWeapon.shotDamage = 15;
		newWeapon.weaponType = WeaponType::glock;
		newWeapon.maxMagAmmo = 12;
		newWeapon.reserveAmmo = 120;

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
		newWeapon.flipXRot = true;

		m_gunVariants.push_back(newWeapon);
	}
}

void GunController::refillMagazine()
{
	if (reserveAmmo > 0 && timeSinceReloadStart <= 0.f && !reloading)
	{
		tiltBackTimeLeft = 0.f;
		tiltDownTimeLeft = 0.f;
		gunModel->modelOffset.position.y = originalPosition;
		gunModel->modelOffset.position.x = originalPositionx;
		gunModel->modelOffset.rotation.x = originalXRot;
		gunModel->modelOffset.rotation.y = originalYRot;

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
	m_weaponType = t_weaponInfo.weaponType;
	shotDamage = t_weaponInfo.shotDamage;
	fireRate = t_weaponInfo.fireRate;
	maxMagAmmo = t_weaponInfo.maxMagAmmo;
	reserveAmmo = t_weaponInfo.reserveAmmo;

	*gunModel = *t_weaponInfo.gunModel;

	gunModel->Start();
	originalYRot = gunModel->modelOffset.rotation.y;
	originalXRot = gunModel->modelOffset.rotation.x;
	originalPosition = gunModel->modelOffset.position.y;
	originalPositionx = gunModel->modelOffset.position.x;

	timeSinceReloadStart = 0.f;
	refillMagazine();
	flipXRot = t_weaponInfo.flipXRot;

	reloadTime = t_weaponInfo.reloadTime;

	fullAuto = t_weaponInfo.fullAuto;
	recoilStrength = t_weaponInfo.recoilStrength;

}
