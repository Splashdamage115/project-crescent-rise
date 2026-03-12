#include "HealthController.h"
#include "GameObject.h"
#include "ParticleController.h"

void HealthController::Start()
{
}

void HealthController::Update()
{
}

void HealthController::sendMessage(const std::string& t_messageString, std::any amount)
{
	if (t_messageString == "DAMAGE" || t_messageString == "HEAL")
	{
		int sign = (t_messageString == "DAMAGE") ? -1 : 1;
		try
		{
			float damageAmount = std::any_cast<float>(amount) * static_cast<float>(sign);
			changeHealth(damageAmount);
		}
		catch (const std::bad_any_cast& e)
		{ }
		try
		{
			int damageAmt = std::any_cast<int>(amount) * sign;
			changeHealth(static_cast<float>(damageAmt));

		}
		catch (const std::bad_any_cast& e)
		{ }
	}
}

void HealthController::changeHealth(float amount)
{
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
	auto p = ParticleController::SpawnNewParticle("./Assets/Images/Particles/blood.png", *transform, 0.2f, glm::vec2(6, 1), glm::vec2(3072.f, 512.f));
	p->positionOverride->scale = glm::vec3(3.5f);

	std::cout << "OBJECT DESTROYED\n";
	parent->active = false;
}
