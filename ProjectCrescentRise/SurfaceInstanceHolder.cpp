#include "SurfaceInstanceHolder.h"
#include "OrientToSurface.h"
#include "Game.h"
#include "jsonPlanetParser.h"

std::vector<std::shared_ptr<SurfaceGrass>> SurfaceInstanceHolder::m_surfaceGrass;
std::vector< std::vector<InstancerSettings>> SurfaceInstanceHolder::m_instancerSettings;

void SurfaceInstanceHolder::init()
{
	//loadInstancer();
	for (int layer = 0; layer < m_instancerSettings.size(); layer++)
	{
		for (int i = 0; i < m_instancerSettings.at(layer).size(); i++)
		{
			SurfaceInstancer instancer1;

			if (!DEBUG_MODE)
			{
				m_instancerSettings.at(layer).at(i).passesPerFace *= 2;
			}

			instancer1.SetSettings(m_instancerSettings.at(layer).at(i));

			m_surfaceGrass.emplace_back(std::make_shared<SurfaceGrass>());
			m_surfaceGrass.back()->grassTextureLoc = m_instancerSettings.at(layer).at(i).textureLoc;

			InstancerSettings x = m_instancerSettings.at(layer).at(i);

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
}

void SurfaceInstanceHolder::loadInstancer()
{
	//std::vector<std::string> grassTextures =
	//{
	//	"./Assets/Images/grass.png",
	//	"./Assets/Images/grass2.png",
	//	"./Assets/Images/grass3.png",
	//	"./Assets/Images/bush.png"
	//};
	//int sizeDecrease = 1;
	//if (DEBUG_MODE) sizeDecrease = 4;
	//
	//std::vector<int> spawnAmt =
	//{
	//	256 / sizeDecrease,
	//	32 / sizeDecrease,
	//	128 / sizeDecrease,
	//	32 / sizeDecrease
	//};
	//
	//for (int i = 0; i < spawnAmt.size(); i++)
	//{
	//	m_instancerSettings.emplace_back();
	//	m_instancerSettings.at(i).textureLoc = grassTextures.at(i);
	//
	//	m_instancerSettings.at(i).density = 1.0f;
	//	m_instancerSettings.at(i).noiseScale = 100.0f;
	//	m_instancerSettings.at(i).noiseThreshold = 10.0f;
	//	m_instancerSettings.at(i).noiseSeed = rand();
	//	m_instancerSettings.at(i).useHeightLayerMask = true;
	//	m_instancerSettings.at(i).heightLayerMask = 2;
	//	m_instancerSettings.at(i).passesPerFace = spawnAmt.at(i);
	//
	//	m_instancerSettings.at(i).maxSize = 1.25f;
	//	m_instancerSettings.at(i).minSize = 0.75f;
	//
	//	m_instancerSettings.at(i).instanceType = InstanceType::Grass;
	//}
	//
	//m_instancerSettings.at(spawnAmt.size() - 1).minSize = 3.75f;
	//m_instancerSettings.at(spawnAmt.size() - 1).maxSize = 4.f;
}

void SurfaceInstanceHolder::drawImGui(int layerAmt)
{
	int inputLength = 128; // length of input boxes

	ImGui::Begin("Surface Instances");

	ImGui::Text("Surface Instances only take Effect on Loading of Scene");

	for (int layer = 0; layer < layerAmt; layer++)
	{
		if (m_instancerSettings.size() < layer)m_instancerSettings.emplace_back();

		std::string t = "";
		t += "Surface Layer ";
		t += std::to_string(layer);

		if (ImGui::TreeNode(t.c_str()))
		{

			if (ImGui::Button("+"))
			{
				InstancerSettings newInst;
				newInst.textureLoc = "./Assets/Images/grass.png";

				newInst.density = 1.0f;
				newInst.noiseScale = 100.0f;
				newInst.noiseThreshold = 10.0f;
				newInst.noiseSeed = rand();
				newInst.useHeightLayerMask = true;
				newInst.heightLayerMask = 2;
				newInst.passesPerFace = 32;

				newInst.maxSize = 1.25f;
				newInst.minSize = 0.75f;

				newInst.instanceType = InstanceType::Grass;

				m_instancerSettings.at(layer).push_back(newInst);

			}

			for (int i = 0; i < m_instancerSettings.at(layer).size(); i++)
			{
				t = "";
				t += "Instance no ";
				std::string loc = ""; loc += std::to_string(layer); loc += "/"; loc += std::to_string(i);
				t += loc;

				if (ImGui::TreeNode(t.c_str()))
				{
					//ImGui::Text("This is a layer");
					t = ""; t += "Texture location "; t += loc;

					ImGui::InputText(t.c_str(), m_instancerSettings.at(layer).at(i).textureLoc.data(), inputLength); 

					t = ""; t += "density "; t += loc;
					ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).density, 0.0f, 1.0f);

					t = ""; t += "noiseScale "; t += loc;
					ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseScale, 0.0f, 300.0f);

					t = ""; t += "noiseThreshold "; t += loc;
					ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseThreshold, 0.0f, 30.0f);

					//t = ""; t += "noiseSeed "; t += loc;
					//ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseSeed, 0.0f, 300.0f);
					
					t = ""; t += "passesPerFace "; t += loc;
					ImGui::SliderInt(t.c_str(), &m_instancerSettings.at(layer).at(i).passesPerFace, 1, 512);

					t = ""; t += "minSize "; t += loc;
					if(ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).minSize, 0.001f, 10.0f))
					{
						if (m_instancerSettings.at(layer).at(i).minSize > m_instancerSettings.at(layer).at(i).maxSize)
						{
							m_instancerSettings.at(layer).at(i).maxSize = m_instancerSettings.at(layer).at(i).minSize;
						}
					}

					t = ""; t += "maxSize "; t += loc;
					if (ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).maxSize, 0.001f, 10.0f))
					{
						if (m_instancerSettings.at(layer).at(i).minSize > m_instancerSettings.at(layer).at(i).maxSize)
						{
							m_instancerSettings.at(layer).at(i).minSize = m_instancerSettings.at(layer).at(i).maxSize;
						}
					}

					//newInst.density = 1.0f;
					//newInst.noiseScale = 100.0f;
					//newInst.noiseThreshold = 10.0f;
					//newInst.noiseSeed = rand();
					//newInst.useHeightLayerMask = true;
					//newInst.heightLayerMask = 2;
					//newInst.passesPerFace = 32;
					//
					//newInst.maxSize = 1.25f;
					//newInst.minSize = 0.75f;


					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	
	}

	ImGui::End();
}
