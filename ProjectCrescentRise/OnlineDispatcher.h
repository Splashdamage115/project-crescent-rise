#pragma once

#include "Library.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

struct PlanetPayload
{
	ShapeSettings shapeSettings;
	PlanetColour planetColour;
	int planetPointCount;
};

class OnlineDispatcher
{
public:
	enum class DispatchType
	{
		ChatText, movement, PlanetData
	};
	static void Init();
	static std::string GetIdentifier();
	static void pushPlanet(const PlanetPayload& t_data);
	static void Dispatch(DispatchType t_type, std::string data);
	static void RecieveDispatch(DispatchType t_type, std::string data);

	static std::vector<char> serializePlanetSettings(const PlanetPayload& t_planetPayload);
	static void interpretCommandsFromServer(std::string data);
	static PlanetPayload deSerializePlanet(const char* data, size_t size);

	static std::string playerIdentifier;
private:
};

