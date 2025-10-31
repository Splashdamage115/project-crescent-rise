#pragma once

#include "RenderableObject.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

class CubeSphere : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;

private:
	int size = 0;
	int pointsPerRow = 24;
	GLint uColourLoc = -1;

	ShapeSettings shapeSettings;
	PlanetColour planetColour;
	ShapeGenerator shapeGenerator;
};
