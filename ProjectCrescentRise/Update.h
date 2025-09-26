#pragma once

#include "Library.h"
#include <functional>

class Update
{
public:
	static void append(const std::function<void(void)>& t_func);
	static void update();
private:
	static std::vector < std::function<void(void)> > m_updateFunctions;
};

