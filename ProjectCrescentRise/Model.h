#pragma once

#include "RenderableObject.h"

class Model : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;

	std::string loadLocation = "./Assets/Mesh/bottle.fbx";
	glm::vec3 colour = glm::vec3(255.f, 192.f, 203.f);
	Transform modelOffset;
private:
	GLint uColourLoc = -1;
	GLuint textureLoc = -1;
	GLuint textureID = -1;
	GLuint heightLoc = -1;
	GLuint heightMapID = -1;
};

