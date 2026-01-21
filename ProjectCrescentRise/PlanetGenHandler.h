#pragma once

#include "Library.h"

class PlanetSurface;
class WaterSphere;
struct PlanetPayload;

class PlanetGenHandler
{
public:
	void init(std::shared_ptr<PlanetSurface>, std::shared_ptr<WaterSphere>);
	void FirstLoadPlanet();
	void guiRender();
	void JsonPlanetSettings();

	int guiWidth = 800;
	int guiHeight = 800;

	void setNewPlanet(PlanetPayload t_payload);

	void update();

	void AlignLiveUpdate();
private:
	void resetPlanet();
	void sendPlanetData();
	void savePlanet(bool saveAsNew);
	void loadPlanet(bool loadNewRandom);

	std::vector< const char* > currentItem;
	std::vector< const char* > currentColour;
	std::vector< std::string > inputTextureAddress;
	std::vector< std::string > inputNormalAddress;
	std::shared_ptr<PlanetSurface> m_planet;
	std::shared_ptr<WaterSphere> m_water;
	bool LiveUpdate = false;
	int colourSelected = 0;

	float currentWaitForSend = 0.0f;
	float MaxWaitForSend = 1.0f;
};

