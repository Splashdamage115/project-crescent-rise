#pragma once

#include "Library.h"

class PlanetSurface;

class PlanetGenHandler
{
public:
	void init(std::shared_ptr<PlanetSurface>);
	void guiRender();

	int guiWidth = 800;
	int guiHeight = 800;
private:
	void resetPlanet();

	std::vector< const char* > currentItem;
	std::shared_ptr<PlanetSurface> m_planet;
	bool LiveUpdate = false;
	int colourSelected = 0;
};

