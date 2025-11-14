#pragma once

#include "Library.h"

class OnlineDispatcher
{
public:
	enum class DispatchType
	{
		ChatText
	};
	static void Init();
	static std::string GetIdentifier();
	static void Dispatch(DispatchType t_type, std::string data);
	static std::string RecieveDispatch();

private:
	static std::string playerIdentifier;
};

