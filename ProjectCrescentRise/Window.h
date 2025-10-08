#pragma once

#include "Library.h"
#include <functional>

class Window
{
public:
	Window();
	~Window();
	bool windowClosed();
	void render();

private:
	GLFWwindow* m_window = nullptr;
	
};

