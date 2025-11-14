#pragma once

#include "Library.h"

class NoiseSettings 
{
public:
	enum class FilterType { Simple, Rigid };

public:
	FilterType filterType = FilterType::Simple;
	float strength = 1.f;
	float roughness = 2.f;
	int numLayers = 1;
	float persistence = 0.5f;
	float baseRoughness = 1.f;
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	float minValue = 0.5f;
	float weightMultiplier = 0.8f;
};