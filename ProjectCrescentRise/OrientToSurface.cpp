#include "OrientToSurface.h"
#include "PlanetSurface.h"
#include "PlayerInput.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

void OrientToSurface::Start()
{
	m_staticRotate = transform->rotation;
	m_firstFrame = true;
}

void OrientToSurface::Update()
{
	if (constantOrient) m_oriented = false;
	if (m_oriented || !transform || !PlanetSurface::s_instance)
		return;

	glm::vec3 currentPos = transform->position;
	glm::vec3 up = PlanetSurface::s_instance->GetSurfaceNormalFromWorldPosition(currentPos);

	glm::vec3 forward;
	if (facePlayer)
	{
		glm::vec3 toPlayer = PlayerInput::playerPosition - currentPos;
		forward = toPlayer - up * glm::dot(toPlayer, up);
		float len = glm::length(forward);
		if (len > 0.001f)
		{
			forward = forward / len;
		}
		else
		{
			glm::vec3 arbitrary = glm::abs(up.x) < 0.9f ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
			forward = glm::normalize(arbitrary - up * glm::dot(arbitrary, up));
		}
	}
	else
	{
		glm::vec3 arbitrary = glm::abs(up.x) < 0.9f ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
		forward = glm::normalize(glm::cross(up, arbitrary));
	}

	glm::vec3 right = glm::normalize(glm::cross(forward, up));
	forward = glm::cross(up, right); 

	glm::mat3 rot(right, up, -forward);

	glm::quat targetRotation = glm::quat_cast(rot);

	glm::quat staticRotQuat = glm::quat(glm::radians(m_staticRotate));
	targetRotation = targetRotation * staticRotQuat;

	if (m_firstFrame)
	{
		m_currentRotation = targetRotation;
		m_firstFrame = false;
	}
	else
	{
		float t = glm::clamp(static_cast<float>(Game::deltaTime) * rotationSmooth, 0.0f, 1.0f);
		m_currentRotation = glm::slerp(m_currentRotation, targetRotation, t);
	}

	glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(m_currentRotation));
	transform->rotation = eulerAngles;

	m_oriented = true;
}
