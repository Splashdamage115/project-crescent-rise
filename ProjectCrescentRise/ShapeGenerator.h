#pragma once

#include "Library.h"
#include "ShapeSettings.h"
#include "NoiseFilter.h"

class ShapeGenerator
{
public:
	ShapeSettings settings;
	std::vector<NoiseFilter> noiseFilters;

	void reset()
	{
		noiseFilters.resize(settings.noiseLayers.size());
		for(int i = 0; i < noiseFilters.size();i++)
			noiseFilters.at(i).init(settings.noiseLayers.at(i).noiseSettings);
	}

	glm::vec3 CalcualtePointOnPlanet(glm::vec3 pointOnUnitSphere)
	{
		float firstLayerValue = 0;
		float elev = 0;

		if (noiseFilters.size() > 0)
		{
			firstLayerValue = noiseFilters.at(0).Evaluate(pointOnUnitSphere);

			if (settings.noiseLayers.at(0).enabled)
				elev = firstLayerValue;
		}
		for (int i = 1; i < noiseFilters.size(); i++)
		{
			if (settings.noiseLayers.at(i).enabled)
			{
				float mask = (settings.noiseLayers.at(i).useAsMask) ? firstLayerValue : 1.f;
				elev += noiseFilters.at(i).Evaluate(pointOnUnitSphere) * mask;
			}
		}
		return pointOnUnitSphere * settings.planetRadius * (1 + elev);
	}
};

