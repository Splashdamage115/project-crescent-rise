#pragma once

#include "Library.h"
#include "ShapeSettings.h"
#include "PlanetColour.h"

struct PlanetData
{
	ShapeSettings planetShape;
	ShapeSettings oceanShapeSettings;
	PlanetColour planetColour;
	int planetPointCount;
	int oceanPointCount;
};

class jsonPlanetParser
{
public:
	static PlanetData RetrieveData();
	static bool WritePlanetData(PlanetData t_newData);
private:

};

