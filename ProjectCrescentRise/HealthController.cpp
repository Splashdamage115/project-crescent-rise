#include "HealthController.h"
#include "GameObject.h"

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
	std::cout << "OBJECT DESTROYED\n";
	parent->active = false;
}
