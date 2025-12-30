#pragma once

#include "RenderableObject.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

class PlanetSurface : public RenderableObject
{
public:
	void ResetPlanet();
	virtual void Start()override;
	virtual void Render()override;

	int pointsPerRow = 4;
	ShapeSettings shapeSettings;
	PlanetColour planetColour;
	ShapeGenerator shapeGenerator;

	// handles if the world is being edited or not
	bool callChange = false;
private:
	int size = 0;
	GLint uColourLoc = -1;
	GLint MinMax = -1;
	GLint CenterPoint = -1;
	GLint heightColours = -1;
	GLint startHeight = -1;
};
