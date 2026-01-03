#pragma once
#include "RenderableObject.h"
class Skybox : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;
private:
	GLint uSkyLoc = -1;
	GLuint textureID = -1;
	GLuint timeID = -1;

};

