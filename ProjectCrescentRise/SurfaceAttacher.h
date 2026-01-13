#pragma once

#include "ScriptObject.h"
#include "PlanetSurface.h"

class SurfaceAttacher : public ScriptObject
{
public:
	SurfaceAttacher() = default;
	
	void SetPlanetSurface(std::shared_ptr<PlanetSurface> surface) 
	{ 
		m_planet = surface;
		m_initialized = true;
		m_lastPlanetChangeState = false;
	}

	virtual void Update() override;

private:
	std::shared_ptr<PlanetSurface> m_planet;
	bool m_initialized = false;
	bool m_lastPlanetChangeState = false;

	void RecalculatePosition();
};
