#pragma once

#include "Library.h"

class CubeSphere;

class PlanetGenHandler
{
public:
	void init(std::shared_ptr<CubeSphere>);
	void guiRender();

	int guiWidth = 800;
	int guiHeight = 800;
private:
	void resetPlanet();

	std::vector< const char* > currentItem;
	std::shared_ptr<CubeSphere> m_planet;
	bool LiveUpdate = false;
	int colourSelected = 0;
};

