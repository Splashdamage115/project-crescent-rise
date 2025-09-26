#include "Update.h"

std::vector < std::function<void(void)> > Update::m_updateFunctions;

void Update::append(const std::function<void(void)>& t_func)
{
	m_updateFunctions.push_back(t_func);
}

void Update::update()
{
	for (auto& func : m_updateFunctions)
	{
		func();
	}
}
