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

std::vector<char> OnlineDispatcher::serializePlanetSettings(const PlanetPayload& t_planetPayload)
{
	std::vector<char> buffer;

	auto write = [&](const void* data, size_t size) {
		const char* bytes = reinterpret_cast<const char*>(data);
		buffer.insert(buffer.end(), bytes, bytes + size);
		};
	uint32_t len = (uint32_t)t_planetPayload.shapeSettings.noiseLayers.size();
	write(&len, sizeof(len));
	write(t_planetPayload.shapeSettings.noiseLayers.data(), len);
	
	float planetRadius = t_planetPayload.shapeSettings.planetRadius;
	write(&planetRadius, sizeof(planetRadius));

	return buffer;
}

PlanetPayload OnlineDispatcher::deSerializePlanet(const char* data, size_t size)
{
	return PlanetPayload();
}

