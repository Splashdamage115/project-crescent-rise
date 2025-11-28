#include "OnlineDispatcher.h"
#include "ChatBoxText.h"
#include "OnlineConnection.h"

std::string OnlineDispatcher::playerIdentifier;

void OnlineDispatcher::Init()
{
	playerIdentifier = "Player 1";
	OnlineConnection::Start();
}

std::string OnlineDispatcher::GetIdentifier()
{
	return (playerIdentifier.size()) ? playerIdentifier : "_UNIDENTIFIED_PLAYER_";
}

void OnlineDispatcher::Dispatch(DispatchType t_type, std::string data)
{
	std::string sendable;

	switch (t_type)
	{
	case OnlineDispatcher::DispatchType::ChatText:
		sendable += data;
		OnlineConnection::pushTcp(sendable);
		break;
	case OnlineDispatcher::DispatchType::movement:
		OnlineConnection::pushPosition(data);
		break;
	default:
		break;
	}
}

void OnlineDispatcher::RecieveDispatch(DispatchType t_type, std::string data)
{
	std::string itemSent = "";
	std::string name = "";
	bool reached = false;
	switch (t_type)
	{
	case OnlineDispatcher::DispatchType::ChatText:
		std::cout << data << std::endl;
		for (auto& l : data)
		{
			if (l != '~' && !reached)
			{
				name += l;
			}
			if (l == '~' && !reached)
			{
				reached = true;
				continue;
			}
			if (reached)
			{
				itemSent += l;
			}
		}
		ChatBoxText::SentNewText(name, itemSent);
		break;
	default:
		break;
	}
}
