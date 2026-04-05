// ***************************************************************************
//
//					How to Use
//
// ***************************************************************************
//
//
// Submit a new command to the append command function
//
//			String arguements are passed throught the string in the function call
//			a call may look like: 
// 
//		   <command> <arguement>
//			/rename coolNewName
//				
//		    CommandInterpreter::append(std::function pointer with string arguement, <command name>);
//
//
//	example Implementation
// 
//		    CommandInterpreter::append([this](std::string t) { this->ClearTextFields(t); }, "/clear");
//			
//
//	after this append call, nothing else is required, except the implementation of the actual function, everything else is handled elsewhere
//


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
	static void append(std::function<void(std::string)> t_func, std::string t_commandCall);
	static bool funcCalled(std::string t_commandCalled);
private:
	static std::vector <commandFunction> m_functions;
};

