#pragma once

#include "Library.h"

class OnlineDispatcher
{
public:
	enum class DispatchType
	{
		ChatText, movement
	};
	static void Init();
	static std::string GetIdentifier();
	static void Dispatch(DispatchType t_type, std::string data);
	static void RecieveDispatch(DispatchType t_type, std::string data);

	static std::string playerIdentifier;
private:
};

