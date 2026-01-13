#include "SurfaceAttacher.h"
#include <iostream>

void SurfaceAttacher::Update()
{
	if (!m_initialized || !m_planet || !transform)
		return;

	bool currentPlanetChangeState = m_planet->callChange;
	
	if (m_lastPlanetChangeState && !currentPlanetChangeState)
	{
		RecalculatePosition();
	}

	m_lastPlanetChangeState = currentPlanetChangeState;
}

void SurfaceAttacher::RecalculatePosition()
{
	if (!m_planet || !transform)
		return;

	glm::vec3 planetCenter = m_planet->getTransform()->position;
	glm::vec3 currentPos = transform->position;
	
	glm::vec3 directionFromCenter = currentPos - planetCenter;
	
	float distanceFromCenter = glm::length(directionFromCenter);
	if (distanceFromCenter < 0.001f)
		return;
	
	glm::vec3 direction = glm::normalize(directionFromCenter);
	
	glm::vec3 testPoint = planetCenter + direction * 100.0f;
	glm::vec3 surfacePoint = m_planet->GetSurfacePointFromWorldPosition(testPoint);
	
	transform->position = surfacePoint;
}
