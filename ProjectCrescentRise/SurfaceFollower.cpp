#include "SurfaceFollower.h"
#include "PlanetSurface.h"
#include "Game.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "KeyScan.h"
#include "PlayerInput.h"

void SurfaceFollower::Start()
{
	m_planet = PlanetSurface::s_instance;
}

void SurfaceFollower::Update()
{
	if (PlayerInput::noClipEnabled)
	{
		return; 
	}

	if (!enabled) return;
	if (!transform) return;
	if (!m_planet)
	{
		m_planet = PlanetSurface::s_instance;
		if (!m_planet) return;
	}

	if (floatOnWater && isUnderwater())
	{
		glm::vec3 playerPos = transform->position;
		float distFromCenter = glm::distance(playerPos, glm::vec3(0.0f));
		
		if (distFromCenter > 0.0f)
		{
			glm::vec3 directionFromCenter = glm::normalize(playerPos);
			glm::vec3 waterSurfacePos = directionFromCenter * PlayerInput::waterHeight;

			float tPos = std::clamp(static_cast<float>(Game::deltaTime) * positionSmooth, 0.0f, 1.0f);
			transform->position = glm::mix(transform->position, waterSurfacePos, tPos);
		}
	}
	else
	{
		
		glm::vec3 surfacePoint = m_planet->GetSurfacePointFromWorldPosition(transform->position);

		glm::vec3 planetCenter = m_planet->getTransform()->position;
		glm::vec3 up = glm::normalize(surfacePoint - planetCenter);
		glm::vec3 desiredPos = surfacePoint + up * heightOffset;

		float tPos = std::clamp(static_cast<float>(Game::deltaTime) * positionSmooth, 0.0f, 1.0f);
		transform->position = glm::mix(transform->position, desiredPos, tPos);
	}
}

bool SurfaceFollower::isUnderwater()
{
	float waterRad = PlayerInput::waterHeight;
	
	if (glm::distance(transform->position, glm::vec3(0.0f)) <= waterRad)
	{
		return true;
	}
	return false;
}
