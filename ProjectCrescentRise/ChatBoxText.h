#pragma once

#include "RenderableObject.h"
#include "KeyScan.h"

class ChatBoxText : public RenderableObject
{
public:
	void Start();
	void Update();
	void Render();

private:
	FT_Face face;
	FT_Library ft;

	GLuint textureID;
	GLint uColourLoc = -1;
	std::string text = "";
	glm::vec4 color = glm::vec4(1.0f);

	bool typing = false;
	std::vector<std::string> m_chatHistory;
};

