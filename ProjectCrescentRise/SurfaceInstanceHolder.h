#pragma once

#include "SurfaceInstancer.h"
#include "SurfaceGrass.h"
#include "Model.h"

class SurfaceInstanceHolder
{
public:
	static void init();
	static void loadInstancer();
	static void drawImGui(int layerAmt);
	static void instanceEnemies(int layer, int count, std::string modelLoc, std::string texLoc);

	static std::vector<std::shared_ptr<SurfaceGrass>> m_surfaceGrass;
	static std::vector<std::shared_ptr<Model>> enemyModels;
	static std::vector< std::shared_ptr<Model>> m_models;
	static std::vector< std::vector<InstancerSettings>> m_instancerSettings;
private:
	static std::vector < std::vector< const char* >> currentInstanceType;
};

