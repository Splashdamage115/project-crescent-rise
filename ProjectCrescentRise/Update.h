






















#pragma once

#include "Library.h"
#include <functional>

class Update
{
public:
	static void append(const std::function<void(void)>& t_func);
	static void appendLate(const std::function<void(void)>& t_func);
	static void update();
	static void clearAll();
private:
	static std::vector < std::function<void(void)> > m_updateFunctions;
	static std::vector < std::function<void(void)> > m_lateUpdateFunctions;
};

