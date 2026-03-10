#pragma once

#include "RenderableObject.h"

class Particle : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Update()override;
	virtual void Render()override;
private:
	GLint uColourLoc = -1;
	GLuint textureLoc = -1;
	GLuint textureID = -1;
	GLuint heightLoc = -1;
	GLuint heightMapID = -1;
	GLuint UVPositionLoc = -1;

	float frameTimeLeft = 0.02f;
	int currentFrame = 0;
};
