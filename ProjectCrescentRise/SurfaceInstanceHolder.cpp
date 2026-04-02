#include "SurfaceInstanceHolder.h"
#include "OrientToSurface.h"
#include "Game.h"
#include "jsonPlanetParser.h"
#include "ModelPartnerScript.h"

std::vector<std::shared_ptr<SurfaceGrass>> SurfaceInstanceHolder::m_surfaceGrass;
std::vector< std::vector<InstancerSettings>> SurfaceInstanceHolder::m_instancerSettings;
std::vector< std::shared_ptr<Model>> SurfaceInstanceHolder::m_models;
std::vector < std::vector< const char* >> SurfaceInstanceHolder::currentInstanceType;

void SurfaceInstanceHolder::init()
{
	//loadInstancer();
	for (int layer = 0; layer < m_instancerSettings.size(); layer++)
	{
		for (int i = 0; i < m_instancerSettings.at(layer).size(); i++)
		{
			SurfaceInstancer instancer1;
			if (m_instancerSettings.at(layer).at(i).instanceType == InstanceType::Grass)
			{

				if (!DEBUG_MODE)
				{
					m_instancerSettings.at(layer).at(i).passesPerFace *= 1;
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
			if (m_instancerSettings.at(layer).at(i).instanceType == InstanceType::PickupModel)
			{
				SurfaceInstancer instancer2;

				InstancerSettings settings2;
				settings2.density = 1.0f;
				settings2.noiseScale = 100.0f;
				settings2.noiseThreshold = 1.0f;
				settings2.noiseSeed = rand();
				settings2.useHeightLayerMask = true;
				settings2.heightLayerMask = 1;
				settings2.passesPerFace = 32;

				instancer2.SetSettings(settings2);

				m_models.emplace_back();

				m_models.at(m_models.size() - 1) = std::make_shared<Model>();
				m_models.at(m_models.size() - 1)->loadLocation = m_instancerSettings.at(layer).at(i).modelLocation;
				m_models.at(m_models.size() - 1)->colour = glm::vec3(133.f, 133.f, 133.f);

				auto creatorFunc2 = []() -> std::shared_ptr<GameObject>
					{
						std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();

						std::shared_ptr<ModelPartnerScript> m = std::make_shared<ModelPartnerScript>();
						obj->tags.emplace_back("interactible");

						m->m_pairedModel = SurfaceInstanceHolder::m_models.at(m_models.size() - 1);
						float grayScale = (rand() % 128) + 64.f;

						m->colour.x = grayScale;
						m->colour.y = grayScale;
						m->colour.z = grayScale;

						obj->addScript(m);
						obj->addScript(SurfaceInstanceHolder::m_models.at(m_models.size() - 1));
						obj->addScript(std::make_shared<OrientToSurface>());
						obj->transform->rotation = { 180.0f, rand() % 180, 0.0f };
						float size = ((rand() % 50) / 100.f) + 0.2f;
						obj->transform->scale = { size , size, size };
						return obj;
					};

				instancer2.InstantiateOnSurface(Game::g_planetScript, creatorFunc2);

			}
			else
			{
				continue;
			}
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
		if (m_instancerSettings.size() < layer) m_instancerSettings.emplace_back();
		if (currentInstanceType.size() < layer) currentInstanceType.emplace_back();

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
				while ((int)(currentInstanceType.at(layer).size()) - 1 < i)
				{
					currentInstanceType.at(layer).push_back("");
				}

				t = "";
				t += "Instance no ";
				std::string loc = ""; loc += std::to_string(layer); loc += "/"; loc += std::to_string(i);
				t += loc;

				if (ImGui::TreeNode(t.c_str()))
				{

						std::string n = "instance type " + loc;
						const char* items[] = { "Grass", "pickupModel" };

						if (ImGui::BeginCombo(n.c_str(), currentInstanceType.at(layer).at(i)))
						{
							for (unsigned int a = 0; a < IM_ARRAYSIZE(items); a++)
							{
								bool isSelected = (currentInstanceType.at(layer).at(i) == items[a]);
						
								if (ImGui::Selectable(items[a], isSelected))
								{
									currentInstanceType.at(layer).at(i) = items[a];
						
									if (currentInstanceType.at(layer).at(i) == "Grass")
									{
										m_instancerSettings.at(layer).at(i).instanceType = InstanceType::Grass;
									}
									if (currentInstanceType.at(layer).at(i) == "pickupModel")
									{
										m_instancerSettings.at(layer).at(i).instanceType = InstanceType::PickupModel;
									}
								}
								if (isSelected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}
						
						
							ImGui::EndCombo();
						}
							


					if (m_instancerSettings.at(layer).at(i).instanceType == InstanceType::PickupModel)
					{

					}


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
					if (ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).minSize, 0.001f, 10.0f))
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





















	//int inputLength = 128; // length of input boxes
	//
	//ImGui::Begin("Surface Instances");
	//
	//ImGui::Text("Surface Instances only take Effect on Loading of Scene");
	//
	//for (int layer = 0; layer < layerAmt; layer++)
	//{
	//	if (m_instancerSettings.size() < layer)
	//	{
	//		m_instancerSettings.emplace_back();
	//	}
	//
	//
	//	//if (currentInstanceType.size() < layer)
	//	//{
	//	//	currentInstanceType.emplace_back();
	//	//}
	//
	//	std::string t = "";
	//	t += "Surface Layer ";
	//	t += std::to_string(layer);
	//
	//	if (ImGui::TreeNode(t.c_str()))
	//	{
	//
	//		if (ImGui::Button("+"))
	//		{
	//			InstancerSettings newInst;
	//			newInst.textureLoc = "./Assets/Images/grass.png";
	//
	//			newInst.density = 1.0f;
	//			newInst.noiseScale = 100.0f;
	//			newInst.noiseThreshold = 10.0f;
	//			newInst.noiseSeed = rand();
	//			newInst.useHeightLayerMask = true;
	//			newInst.heightLayerMask = 2;
	//			newInst.passesPerFace = 32;
	//
	//			newInst.maxSize = 1.25f;
	//			newInst.minSize = 0.75f;
	//
	//			newInst.instanceType = InstanceType::Grass;
	//
	//			m_instancerSettings.at(layer).push_back(newInst);
	//
	//		}
	//
	//		for (int i = 0; i < m_instancerSettings.at(layer).size(); i++)
	//		{
	//			currentInstanceType.at(layer).emplace_back("");
	//
	//			t = "";
	//			t += "Instance no ";
	//			std::string loc = ""; loc += std::to_string(layer); loc += "/"; loc += std::to_string(i);
	//			t += loc;
	//
	//
	//
	//			if (ImGui::TreeNode(t.c_str()))
	//			{
	//				t = ""; t += "Instancer Type "; t += loc;
	//
	//
	//
	//
	//				if (ImGui::CollapsingHeader(t.c_str()))
	//				{
	//					std::string n = "instance type " + loc;
	//					const char* items[] = { "Grass", "pickupModel" };
	//
	//
	//
	//					if (ImGui::BeginCombo(n.c_str(), currentInstanceType.at(layer).at(i)))
	//					{
	//						//	for (unsigned int a = 0; a < IM_ARRAYSIZE(items); a++)
	//						//	{
	//						//		bool isSelected = (currentInstanceType.at(layer).at(i) == items[a]);
	//						//
	//						//		if (ImGui::Selectable(items[a], isSelected))
	//						//		{
	//						//			currentInstanceType.at(layer).at(i) = items[a];
	//						//
	//						//			if (currentInstanceType.at(layer).at(i) == "Grass")
	//						//			{
	//						//				m_instancerSettings.at(layer).at(i).instanceType = InstanceType::Grass;
	//						//			}
	//						//			if (currentInstanceType.at(layer).at(i) == "pickupModel")
	//						//			{
	//						//				m_instancerSettings.at(layer).at(i).instanceType = InstanceType::PickupModel;
	//						//			}
	//						//		}
	//						//		if (isSelected)
	//						//		{
	//						//			ImGui::SetItemDefaultFocus();
	//						//		}
	//						//	}
	//						//
	//						//
	//						//	ImGui::EndCombo();
	//						//}
	//
	//
	//
	//
	//						//ImGui::Text("This is a layer");
	//						t = ""; t += "Texture location "; t += loc;
	//
	//						ImGui::InputText(t.c_str(), m_instancerSettings.at(layer).at(i).textureLoc.data(), inputLength);
	//
	//						t = ""; t += "density "; t += loc;
	//						ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).density, 0.0f, 1.0f);
	//
	//						t = ""; t += "noiseScale "; t += loc;
	//						ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseScale, 0.0f, 300.0f);
	//
	//						t = ""; t += "noiseThreshold "; t += loc;
	//						ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseThreshold, 0.0f, 30.0f);
	//
	//						//t = ""; t += "noiseSeed "; t += loc;
	//						//ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).noiseSeed, 0.0f, 300.0f);
	//
	//						t = ""; t += "passesPerFace "; t += loc;
	//						ImGui::SliderInt(t.c_str(), &m_instancerSettings.at(layer).at(i).passesPerFace, 1, 512);
	//
	//						t = ""; t += "minSize "; t += loc;
	//						if (ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).minSize, 0.001f, 10.0f))
	//						{
	//							if (m_instancerSettings.at(layer).at(i).minSize > m_instancerSettings.at(layer).at(i).maxSize)
	//							{
	//								m_instancerSettings.at(layer).at(i).maxSize = m_instancerSettings.at(layer).at(i).minSize;
	//							}
	//						}
	//
	//						t = ""; t += "maxSize "; t += loc;
	//						if (ImGui::SliderFloat(t.c_str(), &m_instancerSettings.at(layer).at(i).maxSize, 0.001f, 10.0f))
	//						{
	//							if (m_instancerSettings.at(layer).at(i).minSize > m_instancerSettings.at(layer).at(i).maxSize)
	//							{
	//								m_instancerSettings.at(layer).at(i).minSize = m_instancerSettings.at(layer).at(i).maxSize;
	//							}
	//						}
	//
	//						//newInst.density = 1.0f;
	//						//newInst.noiseScale = 100.0f;
	//						//newInst.noiseThreshold = 10.0f;
	//						//newInst.noiseSeed = rand();
	//						//newInst.useHeightLayerMask = true;
	//						//newInst.heightLayerMask = 2;
	//						//newInst.passesPerFace = 32;
	//						//
	//						//newInst.maxSize = 1.25f;
	//						//newInst.minSize = 0.75f;
	//
	//
	//						ImGui::TreePop();
	//					}
	//					ImGui::TreePop();
	//				}
	//
	//			}
	//
	//
	//		}
	//
	//
	//	}
	//	ImGui::End();
	//}
}
