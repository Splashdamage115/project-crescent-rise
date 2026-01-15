#pragma once

#include "Library.h"

class PlanetColour
{
public:
	// copy constructor
	void operator=(const PlanetColour& t_rhs)
	{
		m_shaderType	  = t_rhs.m_shaderType;
		m_textureLocation = t_rhs.m_textureLocation;
		m_textureScale	  = t_rhs.m_textureScale;
		m_normalLocation  = t_rhs.m_normalLocation;
		m_normalStrength  = t_rhs.m_normalStrength;
		m_needsReloading  = t_rhs.m_needsReloading;

		m_colours = t_rhs.m_colours;
		m_heights = t_rhs.m_heights;
		active = t_rhs.active;
		lightIntensity = t_rhs.lightIntensity;
	}

	std::vector<int>		 m_shaderType;
	std::vector<std::string> m_textureLocation;
	std::vector<float>		 m_textureScale;
	std::vector<std::string> m_normalLocation;
	std::vector<float>		 m_normalStrength;
	std::vector<bool>		 m_needsReloading;

	std::vector<glm::vec3> m_colours;
	std::vector<float> m_heights;
	std::vector<bool> active;
	const int COLOUR_MAX = 32;

	float lightIntensity = 0.4f;
};

