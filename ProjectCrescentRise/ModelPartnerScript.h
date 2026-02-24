#pragma once

#include "RenderableObject.h"
#include "Model.h"

class ModelPartnerScript : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Render()override;

	std::shared_ptr<RenderableObject> m_pairedModel;

private:

};

