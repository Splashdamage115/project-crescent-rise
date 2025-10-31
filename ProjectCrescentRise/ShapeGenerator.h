#pragma once

#include "Library.h"
#include "ShapeSettings.h"

class ShapeGenerator
{
public:
	ShapeSettings settings;

	glm::vec3 CalcualtePointOnPlanet(glm::vec3 pointOnUnitSphere)
	{
		return pointOnUnitSphere * settings.planetRadius;
	}
};

