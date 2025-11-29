#pragma once

#include "Library.h"
#include "PlanetColour.h"
#include "ShapeSettings.h"
#include "ShapeGenerator.h"

struct PlanetPayload
{
	ShapeSettings shapeSettings;
	PlanetColour planetColour;
};

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

	static std::vector<char> serializePlanetSettings(const PlanetPayload& t_planetPayload);
	static PlanetPayload deSerializePlanet(const char* data, size_t size);

	static std::string playerIdentifier;
private:
};

