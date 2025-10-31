#pragma once

#include "Library.h"

class NoiseSettings 
{
public:
	float strength = 1.f;
	float roughness = 2.f;
	int numLayers = 1;
	float persistence = 0.5f;
	float baseRoughness = 1.f;
	glm::vec3 center = { 1.0f,1.0f,1.0f };
	float minValue = 0.5f;
};