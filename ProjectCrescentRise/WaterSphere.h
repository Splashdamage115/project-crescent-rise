#pragma once

#include "RenderableObject.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

class WaterSphere : public RenderableObject
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
	GLint MinMax = -1;
	GLint CenterPoint = -1;
	GLint ViewPosition = -1;
	GLint time = -1;
	GLint WaterNormalMap = -1;

	GLuint waterNormalTextureID = -1;
	GLint TextureScale = -1;
	GLint TextureSpeed = -1;

	GLuint waterTextureID = -1;
	GLint WaterColorMap = -1;
};

