#pragma once

#include "RenderableObject.h"

class Particle : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void initialiseData();
	virtual void Update()override;
	virtual void Render()override;

	std::shared_ptr<Transform> positionOverride;
	std::string textureLocation = "./Assets/Images/Particles/basicParticle.png";
	float frameTime = 0.00f;
	glm::vec2 frameAmt = glm::vec2(9, 9);
	glm::vec2 textureSize = glm::vec2(835.f, 796.f);
	glm::vec2 frameSize = glm::vec2(0.f, 0.f);

	bool active = false;
	float maxTimeAlive = 1.f;
private:
	GLint uColourLoc = -1;
	GLuint textureLoc = -1;
	GLuint textureID = -1;
	GLuint heightLoc = -1;
	GLuint heightMapID = -1;
	GLuint UVPositionLoc = -1;

	float frameTimeLeft = 0.0f;
	int currentFrame = 0;

	float timeAliveLeft = 0.f;
};
