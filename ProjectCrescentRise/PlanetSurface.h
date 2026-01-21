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

	// Simple global instance so scripts can sample the planet without heavy coupling.
	// Set in Start(). May be null if no planet exists yet.
	static PlanetSurface* s_instance;

	// Given a world-space position, returns the world-space point on the planet surface
	// along the radial direction from the planet center through worldPos.
	glm::vec3 GetSurfacePointFromWorldPosition(glm::vec3 worldPos);

	// Given a world-space position, returns the world-space normal of the planet surface
	// at the point aligned with the radial direction from the planet center.
	glm::vec3 GetSurfaceNormalFromWorldPosition(glm::vec3 worldPos);
private:
	int size = 0;
	float lightStrength = 0.3f;
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
	GLint ambientLight = -1;

	std::vector<GLuint> textureLocations;
	std::vector<GLuint> normalLocations;
};
