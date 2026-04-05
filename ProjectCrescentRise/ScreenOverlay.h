#pragma once

#include "RenderableObject.h"

class Game;

class ScreenOverlay : public RenderableObject
{
public:
	virtual void Start()override;
	virtual void Update()override;
	virtual void Render()override;
	
	virtual void sendMessage(const std::string& t_messageString)override;

	void EnableGodMode(std::string _);

	Game* mainGame;
private:
	bool dead = false;
	bool deadFadeOut = false;

	glm::vec3 colour = glm::vec3(0.0f);
	glm::vec3 staticOverlay = glm::vec3(0.0f);

	bool staticOverlayActive = false;

	GLint uColourLoc = -1;
	float m_opacity = 1.0f;
	float m_fadeTimeLeft = 3.0f;
	float m_maxFadeTime = 3.0f;

	bool godmode = false;
};