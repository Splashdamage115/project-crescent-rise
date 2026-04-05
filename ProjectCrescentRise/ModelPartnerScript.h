#pragma once

#include "RenderableObject.h"
#include "Model.h"

class ModelPartnerScript : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Update()override;
	virtual void Render()override;

	virtual void sendMessage(const std::string& t_messageType) override;

	float HighlightTimeLeft = -1.f;

	std::shared_ptr<Model> m_pairedModel;
private:

};

