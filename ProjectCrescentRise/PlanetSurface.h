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
	GLint shaderType = -1;
	GLint ViewPosition = -1;
	GLint time = -1;
	GLint layerTextures = -1;
	GLint LayerNormal = -1;
	GLint textureScale = -1;
	GLint NormalStrength = -1;

	std::vector<GLuint> textureLocations;
	std::vector<GLuint> normalLocations;
};
