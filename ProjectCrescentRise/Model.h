#pragma once

#include "RenderableObject.h"
#include "Mover.h"

class Model : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Update() override;
	virtual void Render()override;

	void playAnimation(std::string t_animationName);

	std::string loadLocation = "./Assets/Mesh/bottle.fbx";
	glm::vec3 colour = glm::vec3(255.f, 192.f, 203.f);

	Transform modelOffset;
	bool useOffset = false;
	bool useCameraOffset = false;
	float cameraHeightOffset = 0.0f;
	bool followCam = false;

	bool useOffsetMover = false;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(0.0f);

	std::string textureLoc1 = "";
	std::string textureLoc2 = "";

	bool highlight = false;
	glm::vec3 outlineColor = glm::vec3(255.f, 255.f, 255.f);
	float outlineWidth = 0.01f;
private:
	GLint uColourLoc = -1;
	GLuint textureLoc = -1;
	GLuint textureID = -1;
	GLuint heightLoc = -1;
	GLuint heightMapID = -1;
	GLuint uTexture2Loc = -1;
	GLuint textureID2 = -1;
	GLint highlightLoc = -1;
	GLint viewPosLoc = -1;

	std::shared_ptr<Shader> m_outlineShader;
	GLint outlineModelLoc = -1;
	GLint outlineViewLoc  = -1;
	GLint outlineProjLoc  = -1;
	GLint outlineWidthLoc = -1;
	GLint outlineColorLoc = -1;
};

