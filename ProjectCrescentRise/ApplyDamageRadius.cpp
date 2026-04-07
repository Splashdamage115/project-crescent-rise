#include "ApplyDamageRadius.h"
#include "GameObjects.h"

void ApplyDamageRadius::Start()
{
	auto go = GameObjects::getAllOfTag("Player");
	targetObject = go.at(0);
}

void ApplyDamageRadius::Update()
{
	
	if (glm::distance(transform->position, targetObject->transform->position) < DamageRadius + 1.8f)
	{
		targetObject->sendMessage("DAMAGE", damageAmt);
	}
}
