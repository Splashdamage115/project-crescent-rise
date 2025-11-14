#pragma once

#include "Library.h"

class ChatBox
{
public:
	void init();
	void guiRender();

private:
	std::vector< const char* > m_textHistory;
	char* inputBuffer;
};

