#include "ModelPartnerScript.h"
#include "GameObject.h"

void ModelPartnerScript::Start()
{

}

void ModelPartnerScript::Render()
{
	if (m_pairedModel)
	{
		m_pairedModel->colour = colour;
	}

	if (m_pairedModel)
	{
		std::shared_ptr<Model> modelScript = std::dynamic_pointer_cast<Model>(m_pairedModel);
		if (modelScript) modelScript->highlight = *parent->highlighted;
	}
}
