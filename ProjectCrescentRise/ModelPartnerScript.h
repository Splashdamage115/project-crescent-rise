#pragma once

#include "RenderableObject.h"
#include "Model.h"

class ModelPartnerScript : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;

	std::shared_ptr<Model> m_pairedModel;

	glm::vec3 colour = glm::vec3(255.f, 192.f, 203.f);
private:

};

