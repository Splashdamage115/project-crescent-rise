#pragma once

#include "RenderableObject.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

class CubeSphere : public RenderableObject
{
public:
	void ResetPlanet();
	virtual void Start()override;
	virtual void Render()override;

	int pointsPerRow = 4;
	ShapeSettings shapeSettings;
	PlanetColour planetColour;
	ShapeGenerator shapeGenerator;
private:
	int size = 0;
	GLint uColourLoc = -1;
};
