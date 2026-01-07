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

	glm::vec3 surfacePoint = m_planet->GetSurfacePointFromWorldPosition(transform->position);

	glm::vec3 planetCenter = m_planet->getTransform()->position;
	glm::vec3 up = glm::normalize(surfacePoint - planetCenter);
	glm::vec3 desiredPos = surfacePoint + up * heightOffset;

	float tPos = std::clamp(static_cast<float>(Game::deltaTime) * positionSmooth, 0.0f, 1.0f);
	transform->position = glm::mix(transform->position, desiredPos, tPos);
}
