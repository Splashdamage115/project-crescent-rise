#pragma once

#include "ScriptObject.h"
#include <glm/glm.hpp>

class PlanetSurface;

class SurfaceFollower : public ScriptObject
{
public:
	float heightOffset = 0.5f;

	float positionSmooth = 15.0f;
	float rotationSmooth = 10.0f;

	bool preserveYaw = false;
	bool floatOnWater = false;

	virtual void Start() override;
	virtual void Update() override;

private:
	PlanetSurface* m_planet = nullptr;
	bool isUnderwater();
};
