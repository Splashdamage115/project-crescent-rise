#pragma once
#include "ScriptObject.h"

class OrientToSurface : public ScriptObject
{
public:
	virtual void Start() override;
	virtual void Update() override;

	bool constantOrient = false;
	bool facePlayer = false;
	float rotationSmooth = 5.0f; 
private:
	bool m_oriented = false;
	glm::vec3 m_staticRotate = glm::vec3(0.0f);
	glm::quat m_currentRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	bool m_firstFrame = true;
};
