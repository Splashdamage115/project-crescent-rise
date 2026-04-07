#include "HealthController.h"
#include "GameObject.h"
#include "ParticleController.h"
#include "Game.h"

void HealthController::Start()
{
	currentHealth = maxHealth;
}

void HealthController::Update()
{
	if(invincibilityTime >= 0.f)
		invincibilityTime -= static_cast<float>(Game::deltaTime);
}

void HealthController::sendMessage(const std::string& t_messageString, std::any amount)
{
	if (invincibilityTime > 0.f) return;
	if (parent.expired()) return;
	if (!parent.lock()->active) return;

	if (t_messageString == "DAMAGE" || t_messageString == "HEAL")
	{
		int sign = (t_messageString == "DAMAGE") ? -1 : 1;
		try
		{
			float damageAmount = std::any_cast<float>(amount) * static_cast<float>(sign);
			changeHealth(damageAmount);
			return;
		}
		catch (const std::bad_any_cast& e)
		{ }
		try
		{
			int damageAmt = std::any_cast<int>(amount) * sign;
			changeHealth(static_cast<float>(damageAmt));
			return;
		}
		catch (const std::bad_any_cast& e)
		{ }
	}
}

void HealthController::secondaryHitEffect()
{
	// no secondary here now
}

void HealthController::changeHealth(float amount)
{
	if (parent.expired()) return;
	if (!parent.lock()->active) return;

	std::shared_ptr<Particle> p;

	if (rand() % 6 == 0)
	{
		p = ParticleController::SpawnNewParticle("./Assets/Images/Particles/blood.png", *transform, 0.2f, glm::vec2(6, 1), glm::vec2(3072.f, 512.f), 0.04f);
		p->positionOverride->scale = glm::vec3(3.5f);
	}
	else
	{
		int chosen = (rand() % 5) + 1;
		p = ParticleController::SpawnNewParticle("./Assets/Images/Particles/blood" + std::to_string(chosen) + ".png", *transform, 0.3f, glm::vec2(6, 6), glm::vec2(600.f, 600.f), 0.0f);
		p->positionOverride->scale = glm::vec3(5.f);
	}

	secondaryHitEffect();

	currentHealth += amount;
	if (currentHealth > maxHealth) currentHealth = maxHealth;
	if (currentHealth <= 0.f)
	{
		currentHealth = 0.f;
		expire();
	}
}

void HealthController::expire()
{
	if (parent.expired()) return;
	if (!parent.lock()->active) return;


	auto p = ParticleController::SpawnNewParticle("./Assets/Images/Particles/blood.png", *transform, 0.2f, glm::vec2(6, 1), glm::vec2(3072.f, 512.f), 0.04f);
	p->positionOverride->scale = glm::vec3(3.5f);

	std::cout << "OBJECT DESTROYED\n";
	if (parent.expired()) return;
	parent.lock()->active = false;
}
