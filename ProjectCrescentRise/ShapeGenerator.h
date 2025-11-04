#pragma once

#include "Library.h"
#include "ShapeSettings.h"
#include "NoiseFilters.h"
#include "MinMax.h"

class ShapeGenerator
{
public:
	ShapeSettings settings;
	std::vector<std::shared_ptr<NoiseBaseClass>> noiseFilters;
	MinMax elevationMinMax;

	void reset()
	{
		noiseFilters.resize(settings.noiseLayers.size());
		for (unsigned int i = 0; i < noiseFilters.size(); i++)
		{
			noiseFilters.at(i) = NoiseFilterFactory::getFilter(settings.noiseLayers.at(i).noiseSettings);
			noiseFilters.at(i)->init(settings.noiseLayers.at(i).noiseSettings);
		}
		elevationMinMax = MinMax();
	}

	glm::vec3 CalcualtePointOnPlanet(glm::vec3 pointOnUnitSphere)
	{
		float firstLayerValue = 0;
		float elev = 0;

		if (noiseFilters.size() > 0)
		{
			firstLayerValue = noiseFilters.at(0)->Evaluate(pointOnUnitSphere);

			if (settings.noiseLayers.at(0).enabled)
				elev = firstLayerValue;
		}
		for (unsigned int i = 1; i < noiseFilters.size(); i++)
		{
			if (settings.noiseLayers.at(i).enabled)
			{
				float mask = (settings.noiseLayers.at(i).useAsMask) ? firstLayerValue : 1.f;
				elev += noiseFilters.at(i)->Evaluate(pointOnUnitSphere) * mask;
			}
		}
		elev = settings.planetRadius * (1 + elev);
		elevationMinMax.AddValue(elev);
		return pointOnUnitSphere * elev;
	}
};

