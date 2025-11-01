#include "PlanetGenHandler.h"
#include "CubeSphere.h"
#include "Window.h"
#include <string>

void PlanetGenHandler::init(std::shared_ptr<CubeSphere> t_planet)
{
	m_planet = t_planet;

	// read all info here from a save file
}

void PlanetGenHandler::guiRender()
{
	////////////////////////////////////////////////////////////
	//
	//			Basic Settings
	//
	////////////////////////////////////////////////////////////

	m_planet->callChange = LiveUpdate;

	ImGui::Begin("Planet Settings");
	if (ImGui::Button("Close GUI"))
	{
		Window::Get().closeGUI();
		m_planet->callChange = false;
		return;
	}

	if (ImGui::Button("Update Planet"))
	{
		LiveUpdate = true;
		resetPlanet();
		LiveUpdate = false;
	}

	ImGui::Text("Live Update");
	if (ImGui::Checkbox("Live Update", &LiveUpdate))
	{
		m_planet->callChange = LiveUpdate;
	}
	
	// basic info
	ImGui::Text("Planet Point Count");
	if (ImGui::SliderInt("Planet Point Count", &m_planet->pointsPerRow, 2, 128))
	{
		resetPlanet();
	}
	ImGui::Text("Planet Radius");
	if (ImGui::SliderFloat("Planet Radius", &m_planet->shapeSettings.planetRadius, 0.1f, 256.f))
	{
		resetPlanet();
	}
	ImGui::Text("Planet Colour");
	float col[] = { static_cast<float>(m_planet->planetColour.r) / 255.f, static_cast<float>(m_planet->planetColour.g) / 255.f, static_cast<float>(m_planet->planetColour.b) / 255.f };
	if (ImGui::ColorPicker3("Planet Colour", col))
	{
		if (LiveUpdate)
		{
			m_planet->planetColour.r = col[0] * 255.f;
			m_planet->planetColour.g = col[1] * 255.f;
			m_planet->planetColour.b = col[2] * 255.f;
		}
	}
	ImGui::End();


	////////////////////////////////////////////////////////////
	//
	//			Noise Settings
	//
	////////////////////////////////////////////////////////////

	ImGui::Begin("Noise Settings");

	ImGui::Text("Noise Layers");
	if (ImGui::Button("+"))
	{
		m_planet->shapeSettings.noiseLayers.emplace_back();
	}
	for (int i = 0; i < m_planet->shapeSettings.noiseLayers.size(); i++)
	{
		std::string enable = "Enabled " + std::to_string(i);
		if (ImGui::Checkbox(enable.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).enabled))
		{
			resetPlanet();
		}
		std::string t = "Layer " + std::to_string(i);
		if (ImGui::CollapsingHeader(t.c_str()))
		{
			// base roughness
			std::string n = "Base Roughness " + std::to_string(i);
			if (ImGui::SliderFloat(n.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.baseRoughness, 0.f, 256.f))
			{
				resetPlanet();
			}

			// Center of Noise
			n = "Center " + std::to_string(i);
			float pos[] = { m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.x, m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.y, m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.z };
			if (ImGui::SliderFloat3(n.c_str(), pos, -100.f, 100.f))
			{
				m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.x = pos[0];
				m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.y = pos[1];
				m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.center.z = pos[2];
				resetPlanet();
			}

			// min value (for depth)
			n = "min Value " + std::to_string(i);
			if (ImGui::SliderFloat(n.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.minValue, 0.f, 1.f))
			{
				resetPlanet();
			}

			// Persistence between layers
			n = "Persistence " + std::to_string(i);
			if (ImGui::SliderFloat(n.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.persistence, 0.f, 1.f))
			{
				resetPlanet();
			}

			// Layer Strength
			n = "Strength " + std::to_string(i);
			if (ImGui::SliderFloat(n.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.strength, 0.f, 10.f))
			{
				resetPlanet();
			}
		}
	}

	ImGui::End();
}

void PlanetGenHandler::resetPlanet()
{
	if (LiveUpdate) m_planet->ResetPlanet();
}
