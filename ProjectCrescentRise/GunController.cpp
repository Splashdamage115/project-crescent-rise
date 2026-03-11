#include "GunController.h"
#include "Game.h"
#include "Raycast.h"
#include "GameObjects.h"
#include "ParticleController.h"
#include "Window.h"

void GunController::Start()
{
	std::shared_ptr<keyboardInput> ki = std::make_shared<keyboardInput>();
	ki->active = true;
	ki->keyCode = KeyScan::KeyCode::R;
	ki->function = ([this]() { this->reloadWeapon(); });
	KeyScan::append(ki, true);

	std::shared_ptr<mouseKeyInput> mski = std::make_shared<mouseKeyInput>();
	mski->active = true;
	mski->keyCode = KeyScan::MouseKeyCode::LeftMouse;
	mski->function = ([this]() { this->shootWeapon(); });
	KeyScan::append(mski, true);
}

void GunController::Update()
{
	if(timeSinceLastShot > 0.f) timeSinceLastShot -= static_cast<float>(Game::deltaTime);
	if(timeSinceReloadStart > 0.f) timeSinceReloadStart -= static_cast<float>(Game::deltaTime);
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
		refillMagazine();
	}
}

void GunController::reloadWeapon()
{
	if(magAmmo != maxMagAmmo)
		refillMagazine();
}

void GunController::setGunModel(std::shared_ptr<Model> model)
{ 
	gunModel = model;

	muzzleOffset.position = glm::vec3(-0.2f, 0.1f, -0.5f);
}

void GunController::refillMagazine()
{
	if (reserveAmmo > 0 && timeSinceReloadStart <= 0.f)
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
