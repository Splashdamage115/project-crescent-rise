#include "CommandInterpreter.h"

std::vector <commandFunction> CommandInterpreter::m_functions;

void CommandInterpreter::append(std::function<void(std::string)> t_func, std::string t_comm)
{
	commandFunction t;
	t.function = t_func;
	t.command = t_comm;
	m_functions.emplace_back(t);
}

bool CommandInterpreter::funcCalled(std::string t_commandCalled)
{
	std::string commandUpToSpace;
	std::string arguement;

	bool arg1 = false;
	for (auto& l : t_commandCalled)
	{
		if (l != ' ')
		{
			if (arg1)
			{
				arguement += l;
			}
			else
			{
				commandUpToSpace += l;
			}
		}
		else
		{
			if (arg1) break;

			arg1 = true;
		}
	}
	for (auto& t : m_functions)
	{
		if (t.command != commandUpToSpace) continue;
		try
		{

			t.function(arguement);
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
