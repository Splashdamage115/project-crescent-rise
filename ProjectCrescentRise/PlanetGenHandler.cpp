#include "PlanetGenHandler.h"
#include "CubeSphere.h"

void PlanetGenHandler::init(std::shared_ptr<CubeSphere> t_planet)
{
	m_planet = t_planet;
}

void PlanetGenHandler::guiRender()
{
	ImGui::Begin("Im gui window is here Hi!");
	if (ImGui::Button("Change Colour"))
	{
		m_planet->planetColour.r = 0;
		m_planet->ResetPlanet();
	}
	if (ImGui::SliderInt("Change Planet point count", &m_planet->pointsPerRow, 2, 255))
	{
		m_planet->ResetPlanet();
	}
	ImGui::End();
}
