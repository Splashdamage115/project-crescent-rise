#include "Update.h"

std::vector < std::function<void(void)> > Update::m_updateFunctions;
std::vector < std::function<void(void)> > Update::m_lateUpdateFunctions;

void Update::append(const std::function<void(void)>& t_func)
{
	m_updateFunctions.push_back(t_func);
}

void Update::appendLate(const std::function<void(void)>& t_func)
{
	m_lateUpdateFunctions.push_back(t_func);
}

void Update::update()
{
	for (int i = 0; i < m_updateFunctions.size(); i++)
	{
		m_updateFunctions.at(i)();
	}
	for (int i = 0; i < m_lateUpdateFunctions.size(); i++)
	{
		m_lateUpdateFunctions.at(i)();
	}
}
