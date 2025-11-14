#include "OnlineDispatcher.h"

std::string OnlineDispatcher::playerIdentifier;

void OnlineDispatcher::Init()
{
	playerIdentifier = "Player 1";
}

std::string OnlineDispatcher::GetIdentifier()
{
	return (playerIdentifier.size()) ? playerIdentifier : "_UNIDENTIFIED_PLAYER_";
}

void OnlineDispatcher::Dispatch(DispatchType t_type, std::string data)
{
	switch (t_type)
	{
	case OnlineDispatcher::DispatchType::ChatText:

		break;
	default:
		break;
	}
}

std::string OnlineDispatcher::RecieveDispatch()
{
	return std::string();
}
