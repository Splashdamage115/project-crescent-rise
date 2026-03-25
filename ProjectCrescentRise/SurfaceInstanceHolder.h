#pragma once

#include "SurfaceInstancer.h"
#include "SurfaceGrass.h"

class SurfaceInstanceHolder
{
public:
	static void init();
	static void loadInstancer();
	static void drawImGui(int layerAmt);

	static std::vector<std::shared_ptr<SurfaceGrass>> m_surfaceGrass;
	static std::vector< std::vector<InstancerSettings>> m_instancerSettings;
private:
};

