#pragma once

#include "RenderableObject.h"
#include "KeyScan.h"

struct ChatText
{
	std::string text = "";
	std::string playerName = "";
	float TimeOut = 1.0f;
};

class ChatBoxText : public RenderableObject
{
public:
	void Start();
	void Update();
	void Render();

	static void SentNewText(std::string text, std::string name);

	// debug function
	void ClearTextFields(std::string t_arguement);
	void ChangePlayerName(std::string t_arguement);
	void changeDebug(std::string _);


	bool typing = false;
private:
	void CleanRender();
	void RenderTexts(std::string t_textToRender, float x, float y);

	std::string m_playerName = "Player1";

	FT_Face face;
	FT_Library ft;

	GLuint textureID;
	GLint uColourLoc = -1;
	std::string text = "";
	glm::vec4 color = glm::vec4(1.0f);

	static std::vector<ChatText> m_chatHistory;
};

