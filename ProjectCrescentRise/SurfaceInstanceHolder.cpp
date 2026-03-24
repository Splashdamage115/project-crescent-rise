#include "SurfaceInstanceHolder.h"
#include "OrientToSurface.h"
#include "Game.h"
#include "jsonPlanetParser.h"

std::vector<std::shared_ptr<SurfaceGrass>> SurfaceInstanceHolder::m_surfaceGrass;
std::vector<InstancerSettings> SurfaceInstanceHolder::m_instancerSettings;

void SurfaceInstanceHolder::init()
{
	//loadInstancer();
	for (int i = 0; i < m_instancerSettings.size(); i++)
	{
		SurfaceInstancer instancer1;

		if (!DEBUG_MODE)
		{
			m_instancerSettings.at(i).passesPerFace *= 2;
		}

		instancer1.SetSettings(m_instancerSettings.at(i));

		m_surfaceGrass.emplace_back(std::make_shared<SurfaceGrass>());
		m_surfaceGrass.back()->grassTextureLoc = m_instancerSettings.at(i).textureLoc;

		InstancerSettings x = m_instancerSettings.at(i);

		auto creatorFunc = [x]() -> std::shared_ptr<GameObject>
			{
				std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
				obj->addScript(SurfaceInstanceHolder::m_surfaceGrass.back());
				obj->addScript(std::make_shared<OrientToSurface>());
				obj->transform->rotation = { 0.0f, 0.0f, 180.0f };
				int heightOffset = (x.maxSize - x.minSize) * 100;
				float randScale = ((rand() % heightOffset) / 100.f) + x.minSize;
				float extraHeight = ((rand() % 50) / 100.f);
				obj->transform->scale = { randScale, randScale + extraHeight, randScale };
				return obj;
			};

		instancer1.InstantiateOnSurface(Game::g_planetScript, creatorFunc);
	}
}

void SurfaceInstanceHolder::loadInstancer()
{
	std::vector<std::string> grassTextures =
	{
		"./Assets/Images/grass.png",
		"./Assets/Images/grass2.png",
		"./Assets/Images/grass3.png",
		"./Assets/Images/bush.png"
	};
	int sizeDecrease = 1;
	if (DEBUG_MODE) sizeDecrease = 4;

	std::vector<int> spawnAmt =
	{
		256 / sizeDecrease,
		32 / sizeDecrease,
		128 / sizeDecrease,
		32 / sizeDecrease
	};

	for (int i = 0; i < spawnAmt.size(); i++)
	{
		m_instancerSettings.emplace_back();
		m_instancerSettings.at(i).textureLoc = grassTextures.at(i);

		m_instancerSettings.at(i).density = 1.0f;
		m_instancerSettings.at(i).noiseScale = 100.0f;
		m_instancerSettings.at(i).noiseThreshold = 10.0f;
		m_instancerSettings.at(i).noiseSeed = rand();
		m_instancerSettings.at(i).useHeightLayerMask = true;
		m_instancerSettings.at(i).heightLayerMask = 2;
		m_instancerSettings.at(i).passesPerFace = spawnAmt.at(i);

		m_instancerSettings.at(i).maxSize = 1.25f;
		m_instancerSettings.at(i).minSize = 0.75f;

		m_instancerSettings.at(i).instanceType = InstanceType::Grass;
	}

	m_instancerSettings.at(spawnAmt.size() - 1).minSize = 3.75f;
	m_instancerSettings.at(spawnAmt.size() - 1).maxSize = 4.f;
}

void SurfaceInstanceHolder::drawImGui()
{
}
