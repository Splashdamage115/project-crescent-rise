#pragma once

#include "ScriptObject.h"
#include <glm/glm.hpp>

class PlanetSurface;

// Scriptable component to make any object "stick" to the planet surface.
// Usage: add to a GameObject as a script; it will sample the nearest point on the active PlanetSurface (PlanetSurface::s_instance).
class SurfaceFollower : public ScriptObject
{
public:
	// meters above the surface
	float heightOffset = 0.5f;

	// smoothing (higher = snappier)
	float positionSmooth = 15.0f;
	float rotationSmooth = 10.0f;

	// If true preserve the object's yaw and compute pitch to align with the surface tangent.
	// If false the follower will try to align full orientation to surface (not implemented here).
	bool preserveYaw = true;

	// Script lifecycle
	virtual void Start() override;
	virtual void Update() override;

private:
	PlanetSurface* m_planet = nullptr;
};
