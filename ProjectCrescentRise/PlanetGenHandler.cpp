#include "PlanetGenHandler.h"
#include "PlanetSurface.h"
#include "WaterSphere.h"
#include "Window.h"
#include <string>
#include "OnlineDispatcher.h"
#include "Update.h"
#include "Game.h"

void PlanetGenHandler::init(std::shared_ptr<PlanetSurface> t_planet, std::shared_ptr<WaterSphere> t_water)
{
	m_planet = t_planet;
	m_water = t_water;

	Update::append([this]() { this->update(); });
	// read all info here from a save file
}

void PlanetGenHandler::guiRender()
{
	////////////////////////////////////////////////////////////
	//
	//			Basic Settings
	//
	////////////////////////////////////////////////////////////

	int inputLength = 128; // length of input boxes

	m_planet->callChange = LiveUpdate;
	m_water->callChange = LiveUpdate;

	ImGui::Begin("Planet Settings");
	if (ImGui::Button("Close GUI"))
	{
		Window::Get().closeGUI();
		m_planet->callChange = false;
		m_water->callChange = false;
		LiveUpdate = false;
		return;
	}

	if (ImGui::Button("Send Planet"))
	{
		sendPlanetData();
	}

	if (ImGui::Button("Update Planet"))
	{
		sendPlanetData();
		LiveUpdate = true;
		resetPlanet();
		LiveUpdate = false;
	}

	ImGui::Text("Live Update");
	if (ImGui::Checkbox("Live Update", &LiveUpdate))
	{
		m_planet->callChange = LiveUpdate;
		m_water->callChange = LiveUpdate;
	}
	
	// basic info
	ImGui::Text("Planet Point Count");
	if (ImGui::SliderInt("Planet Point Count", &m_planet->pointsPerRow, 2, 128))
	{
		resetPlanet();
	}
	if (ImGui::SliderInt("Water Point Count", &m_water->pointsPerRow, 2, 128))
	{
		resetPlanet();
	}
	ImGui::Text("Planet Radius");
	if (ImGui::SliderFloat("Planet Radius", &m_planet->shapeSettings.planetRadius, 0.1f, 256.f))
	{
		resetPlanet();
	}
	if (ImGui::SliderFloat("Water Radius", &m_water->shapeSettings.planetRadius, 0.1f, 256.f))
	{
		resetPlanet();
	}
	ImGui::Text("Planet Colours");

	if (ImGui::SliderFloat("Light intensity", &m_planet->planetColour.lightIntensity, 0.0f, 1.f))
	{

	}

	// planet colour layers
	if (ImGui::Button("+"))
	{
		m_planet->planetColour.active.at(colourSelected) = true;
		colourSelected++;
	}
	for (int i = 0; i < m_planet->planetColour.COLOUR_MAX; i++) {
		if (!m_planet->planetColour.active.at(i)) continue;


		if (i - 1 > m_planet->planetColour.active.size()) 
		{ 
			inputTextureAddress.emplace_back(m_planet->planetColour.m_textureLocation.at(i));
			inputNormalAddress.emplace_back(m_planet->planetColour.m_normalLocation.at(i));
			currentColour.emplace_back();
		}

		std::string t = "Colour " + std::to_string(i);
		float col[] = { static_cast<float>(m_planet->planetColour.m_colours.at(i).x) / 255.f, static_cast<float>(m_planet->planetColour.m_colours.at(i).y) / 255.f, static_cast<float>(m_planet->planetColour.m_colours.at(i).z) / 255.f };
		std::string text = "Visible colour " + std::to_string(i) + ":";


		if (m_planet->planetColour.m_shaderType.at(i) == 0)
		{
			// name for the colour, mainly for seperation between colours
			ImGui::Text(text.c_str());

			// small indicator to show the layers colour, this is to help read the item, and know which it is
			text = "actual colour " + std::to_string(i);
			ImGui::ColorButton(text.c_str(), ImVec4(col[0], col[1], col[2], 255.0f));
		}



		// collapsing header to hiade all other elements
		if (ImGui::CollapsingHeader(t.c_str()))
		{
			std::string n = "Layer Overlay Type " + std::to_string(i);
			const char* items[] = { "Colour", "Texture" };



			if (ImGui::BeginCombo(n.c_str(), currentColour.at(i)))
			{
				for (unsigned int a = 0; a < IM_ARRAYSIZE(items); a++)
				{
					bool isSelected = (currentColour.at(i) == items[a]);

					if (ImGui::Selectable(items[a], isSelected))
					{
						currentColour.at(i) = items[a];

						if (currentColour.at(i) == "Colour")
						{
							m_planet->planetColour.m_shaderType.at(i) = 0;
							resetPlanet();
						}
						if (currentColour.at(i) == "Texture")
						{
							m_planet->planetColour.m_shaderType.at(i) = 1;
							resetPlanet();
						}
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}


				ImGui::EndCombo();

			}

			// control the layer height
			text = "layer height adjustment " + std::to_string(i);
			if (ImGui::SliderFloat(text.c_str(), &m_planet->planetColour.m_heights.at(i), 0.f, 1.f))
			{
				resetPlanet();
			}


			if (m_planet->planetColour.m_shaderType.at(i) == 0)
			{
				// change the colour on that layer
				text = "Layer Colour " + std::to_string(i);
				if (ImGui::ColorPicker3(text.c_str(), col))
				{
					if (LiveUpdate)
					{
						m_planet->planetColour.m_colours.at(i).x = static_cast<unsigned short>(col[0] * 255.f);
						m_planet->planetColour.m_colours.at(i).y = static_cast<unsigned short>(col[1] * 255.f);
						m_planet->planetColour.m_colours.at(i).z = static_cast<unsigned short>(col[2] * 255.f);

					}
				}
			}
			else if (m_planet->planetColour.m_shaderType.at(i) == 1)
			{
				text = "Texture " + std::to_string(i);
				ImGui::Text(text.c_str());

				text = "Load Texture " + std::to_string(i);
				if (ImGui::Button(text.c_str()))
				{
					m_planet->planetColour.m_needsReloading.at(i) = true;
					resetPlanet();
				}

				text = "Texture Address " + std::to_string(i);
				if (ImGui::InputText(text.c_str(), inputTextureAddress.at(i).data(), inputLength))
				{
					m_planet->planetColour.m_textureLocation.at(i) = inputTextureAddress.at(i);
				}

				text = "Texture Tiling Density " + std::to_string(i);
				if (ImGui::SliderFloat(text.c_str(), &m_planet->planetColour.m_textureScale.at(i), 0.001f, 256.f))
				{
					
				}

				text = "Normal Address " + std::to_string(i);
				if (ImGui::InputText(text.c_str(), inputNormalAddress.at(i).data(), inputLength))
				{
					m_planet->planetColour.m_normalLocation.at(i) = inputNormalAddress.at(i);
				}

				text = "Normal Strength " + std::to_string(i);
				if (ImGui::SliderFloat(text.c_str(), &m_planet->planetColour.m_normalStrength.at(i), -5.0f, 5.0f))
				{

				}
			}




		}
	}
	ImGui::End();


	////////////////////////////////////////////////////////////
	//
	//			Noise Settings
	//
	////////////////////////////////////////////////////////////

	ImGui::Begin("Noise Settings");

	// add a new noise leayer
	ImGui::Text("Noise Layers");
	if (ImGui::Button("+"))
	{
		m_planet->shapeSettings.noiseLayers.emplace_back();
	}
	for (unsigned int i = 0; i < m_planet->shapeSettings.noiseLayers.size(); i++)
	{
		if (i - 1 > currentItem.size()) currentItem.emplace_back();

		std::string enable = "Enabled " + std::to_string(i);
		if (ImGui::Checkbox(enable.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).enabled))
		{
			resetPlanet();
		}
		std::string t = "Layer " + std::to_string(i);
		if (ImGui::CollapsingHeader(t.c_str()))
		{
			std::string n = "Noise Filter Type " + std::to_string(i);
			const char* items[] = { "Simple", "Rigid" };
			

			if (ImGui::BeginCombo(n.c_str(), currentItem.at(i))) 
			{
				for (unsigned int a = 0; a < IM_ARRAYSIZE(items); a++) 
				{
					bool isSelected = (currentItem.at(i) == items[a]);

					if (ImGui::Selectable(items[a], isSelected))
					{
						currentItem.at(i) = items[a];

						if (currentItem.at(i) == "Simple")
						{
							m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.filterType = NoiseSettings::FilterType::Simple;
							resetPlanet();
						}
						if (currentItem.at(i) == "Rigid")
						{
							m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.filterType = NoiseSettings::FilterType::Rigid;
							resetPlanet();
						}
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				
				ImGui::EndCombo();

			}

			// Layer Amount
			n = "Layer Amount " + std::to_string(i);
			if (ImGui::SliderInt(n.c_str(), &m_planet->shapeSettings.noiseLayers.at(i).noiseSettings.numLayers, 1, 30))
			{
				resetPlanet();
			}

			// base roughness
			n = "Base Roughness " + std::to_string(i);
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

void PlanetGenHandler::setNewPlanet(PlanetPayload t_payload)
{
	for(int i =0; i < t_payload.planetColour.m_colours.size();i++)
		m_planet->planetColour.m_colours.at(i) = t_payload.planetColour.m_colours.at(i);
	for (int i = 0; i < t_payload.planetColour.m_heights.size(); i++)
		m_planet->planetColour.m_heights = t_payload.planetColour.m_heights;

	m_planet->shapeSettings.noiseLayers = t_payload.shapeSettings.noiseLayers;
	m_planet->shapeSettings.planetRadius = t_payload.shapeSettings.planetRadius;

	m_planet->pointsPerRow = t_payload.planetPointCount;

	if (!LiveUpdate)
	{
		LiveUpdate = true;
		resetPlanet();
		LiveUpdate = false;
	}
}

void PlanetGenHandler::update()
{
	if (LiveUpdate)
	{
		currentWaitForSend -= Game::deltaTime;

		if (currentWaitForSend <= 0.0f)
		{
			currentWaitForSend = MaxWaitForSend;

			sendPlanetData();
		}
	}
	
}

void PlanetGenHandler::resetPlanet()
{
	if (LiveUpdate) 
	{ 
		m_planet->ResetPlanet();
		m_water->ResetPlanet();
	}
}

void PlanetGenHandler::sendPlanetData()
{
	PlanetPayload p;

	p.planetColour.m_colours = m_planet->planetColour.m_colours;
	p.planetColour.m_heights = m_planet->planetColour.m_heights;

	p.shapeSettings.noiseLayers = m_planet->shapeSettings.noiseLayers;
	p.shapeSettings.planetRadius = m_planet->shapeSettings.planetRadius;

	p.planetPointCount = m_planet->pointsPerRow;

	OnlineDispatcher::pushPlanet(p);
}
