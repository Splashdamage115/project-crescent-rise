#pragma once
#include "RenderableObject.h"

class Cube : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;
private:
	GLint uColourLoc = -1;
	GLuint textureLoc = -1;
	GLuint textureID = -1;
	GLuint heightLoc = -1;
	GLuint heightMapID = -1;
};

