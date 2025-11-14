#include "ChatBox.h"

void ChatBox::init()
{
	m_textHistory.emplace_back("Player1 > Hello world");
	m_textHistory.emplace_back("Player1 > Hello world");
	m_textHistory.emplace_back("Player1 > Hello world");
	m_textHistory.emplace_back("Player1 > Hello world");
	m_textHistory.emplace_back("Player1 > Hello world");

	inputBuffer = new char();
}

void ChatBox::guiRender()
{
	ImGui::Begin("Text Box");
	for (int i = 0; i < m_textHistory.size(); i++)
	{
		ImGui::Text(m_textHistory.at(i));
	}
	const char* input = "Name";
	if(ImGui::InputText(input, inputBuffer, 256))
	{
	}
	if (ImGui::Button(">"))
	{
		std::string t = "Player1 > ";
		t += inputBuffer;
		m_textHistory.emplace_back(t.c_str());
		inputBuffer = new char();
	}
	ImGui::End();
}
