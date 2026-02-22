#include "ModelPartnerScript.h"

void ModelPartnerScript::Start()
{

}

void ModelPartnerScript::Render()
{
	if(m_pairedModel)
		m_pairedModel->colour = colour;

	m_pairedModel->highlight = *parentHighlight;
}
