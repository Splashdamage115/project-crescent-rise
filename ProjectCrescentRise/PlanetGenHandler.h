#pragma once

#include "Library.h"

class PlanetSurface;
struct PlanetPayload;

class PlanetGenHandler
{
public:
	void init(std::shared_ptr<PlanetSurface>);
	void guiRender();

	int guiWidth = 800;
	int guiHeight = 800;

	void setNewPlanet(PlanetPayload t_payload);

	void update();
private:
	void resetPlanet();
	void sendPlanetData();

	std::vector< const char* > currentItem;
	std::shared_ptr<PlanetSurface> m_planet;
	bool LiveUpdate = false;
	int colourSelected = 0;

	float currentWaitForSend = 0.0f;
	float MaxWaitForSend = 1.0f;
};

