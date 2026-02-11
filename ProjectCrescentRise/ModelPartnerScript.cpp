#include "ModelPartnerScript.h"

void ModelPartnerScript::Start()
{
	float grayScale = (rand() % 128) + 64.f;
	colour.x = grayScale;
	colour.y = grayScale;
	colour.z = grayScale;
}

void ModelPartnerScript::Render()
{
	if(m_pairedModel)
		m_pairedModel->colour = colour;
}
