#pragma once

#include "Library.h"

class PlanetColour
{
public:
	std::vector<int> m_shaderType;
	std::vector<std::string> m_textureLocation;
	std::vector<float> m_textureScale;
	std::vector<std::string> m_normalLocation;
	std::vector<float> m_normalStrength;
	std::vector<bool> m_needsReloading;

	std::vector<glm::vec3> m_colours;
	std::vector<float> m_heights;
	std::vector<bool> active;
	const int COLOUR_MAX = 32;

};

