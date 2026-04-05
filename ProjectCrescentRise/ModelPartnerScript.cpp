#include "ModelPartnerScript.h"
#include "GameObject.h"
#include "Game.h"

void ModelPartnerScript::Start()
{

}

void ModelPartnerScript::Update()
{
	if (HighlightTimeLeft >= 0.f)
	{
		HighlightTimeLeft -= static_cast<float>(Game::deltaTime);
	}
}

void ModelPartnerScript::Render()
{
	if (m_pairedModel)
	{
		m_pairedModel->colour = colour;
	}

	if (HighlightTimeLeft >= 0.f)
	{
		m_pairedModel->hitFlash = true;
	}
	m_pairedModel->attackHighlight = charging;

	if (m_pairedModel)
	{
		std::shared_ptr<Model> modelScript = std::dynamic_pointer_cast<Model>(m_pairedModel);
		if (modelScript) modelScript->highlight = *parent->highlighted;
	}
	charging = false;
}

void ModelPartnerScript::sendMessage(const std::string& t_messageType)
{
	if (t_messageType == "DAMAGE")
	{
		HighlightTimeLeft = 0.2f;
	}
	if (t_messageType == "charging")
	{
		charging = true;
	}
}
