#pragma once

#include "Library.h"
#include "ShapeSettings.h"
#include "PlanetColour.h"
#include "json.hpp"

using json = nlohmann::json;

struct PlanetData
{
	json planetDataToJson();
	static PlanetData jsonToPlanetData(const json& input);


	ShapeSettings planetShape;
	ShapeSettings oceanShapeSettings;
	PlanetColour planetColour;
	int planetPointCount;
	int oceanPointCount;
};

class jsonPlanetParser
{
public:
	static PlanetData RetrieveData(bool newRandom);
	static bool WritePlanetData(PlanetData t_newData, bool saveAsNew);
	static int s_chosenPlanet;
	static int s_amountOfPlanets;
private:
	static json readPlanetFile();
	static void MergeFiles(json& base, json mergeFile);
	static bool dumpFile(const json& dumpFile);

	static const std::string s_fileLocation;
};

