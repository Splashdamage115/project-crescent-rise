#include "CommandInterpreter.h"

std::vector <commandFunction> CommandInterpreter::m_functions;

void CommandInterpreter::append(std::function<void(void)> t_func, std::string t_comm)
{
	commandFunction t;
	t.function = t_func;
	t.command = t_comm;
	m_functions.emplace_back(t);
}

bool CommandInterpreter::funcCalled(std::string t_commandCalled)
{
	for (auto& t : m_functions)
	{
		if (t.command != t_commandCalled) continue;
		try
		{
			t.function();
			return true;
		}
		catch(int errorCode)
		{
			std::cerr << "INVALID FUNCTION CALLED : " << t_commandCalled << "\n";
			return false;
		}
	}
	return false;
}
