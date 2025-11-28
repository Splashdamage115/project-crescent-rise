#pragma once

#include "Library.h"
#include <functional>

struct commandFunction
{
	std::function<void(std::string)> function;
	std::string command;
};

class CommandInterpreter
{
public:
	static void append(std::function<void(std::string)> t_func, std::string t_comm);
	static bool funcCalled(std::string t_commandCalled);
private:
	static std::vector <commandFunction> m_functions;
};

