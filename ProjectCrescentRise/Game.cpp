#include "Game.h"
#include "OnlineDispatcher.h"
#include "Update.h"
#include "Mover.h"
#include "billboard2D.h"
#include "Window.h"
#include "CameraFeed.h"
#include "PlayerInput.h"
#include "GroundTile.h"
#include "PlanetSurface.h"
#include "ChatBoxText.h"
#include "CubeSphere.h"
#include "Skybox.h"
#include "Cube.h"
#include "WaterSphere.h"
#include "SurfaceFollower.h"
#include "SurfaceInstancer.h"
#include "SurfaceAttacher.h"
#include "OrientToCenter.h"
#include "OrientToSurface.h"
#include "Model.h"
#include "SurfaceGrass.h"
#include "ModelPartnerScript.h"
#include "GunAttachment.h"
#include "HealthController.h"
#include "GunController.h"
#include "EnemyMovement.h"
#include "Particle.h"
#include "ParticleController.h"
#include "EnemyStateManager.h"
#include "SurfaceInstanceHolder.h"
#include "mainMenu.h"
#include "PlayerHealthController.h"
#include "ScreenOverlay.h"

double Game::deltaTime = 0;
std::shared_ptr<PlanetSurface> Game::g_planetScript; 

Game::Game()
{
	Window::Get();
	initMainMenu();
	//initGame();
}

void Game::initMainMenu()
{
	Update::clearAll();
	GameObjects::clearAll();

	Window::Get().initMenu();

	mainMenuObj = std::make_shared<GameObject>();
	auto t = std::make_shared<mainMenu>();

	t->mainGame = this;

	mainMenuObj->addScript(t);
	GameObjects::addNewObjectToPool(mainMenuObj);
}

void Game::initGame()
{
	Update::clearAll();
	GameObjects::clearAll();

	if (mainMenuObj != nullptr && mainMenuObj->active == true)
	{
		mainMenuObj->sendMessage("DestroyContext");
		mainMenuObj->active = false;
		//mainMenuObj = nullptr;
	}

	OnlineDispatcher::Init();

	Window::Get().StartGame();
	Window::Get().InitCamera();


	// - - - SCREEN OVERLAY - - -
	overlayObj = std::make_shared<GameObject>();
	auto overlay = std::make_shared<ScreenOverlay>();
	overlay->mainGame = this;
	overlayObj->addScript(overlay);
	overlayObj->tags.emplace_back("overlay");
	GameObjects::addNewObjectToPool(overlayObj);
	// - - - !SCREEN OVERLAY - - -
	
	// - - - PLAYER - - - 

	std::shared_ptr<Model> gunModel = std::make_shared<Model>();

	auto cameraFeed = std::make_shared<CameraFeed>();

	camObj = std::make_shared<GameObject>();
	camObj->transform->position = { 50.f, 0.f, 0.f };
	camObj->transform->rotation = { 0.0f, 0.0f, 0.0f };
	camObj->addScript(cameraFeed);
	camObj->addScript(std::make_shared<PlayerInput>());

	camObj->addScript(std::make_shared<SurfaceFollower>());
	//camObj->addScript(std::make_shared<CubeSphere>());

	auto g = std::make_shared<GunController>();
	g->setGunModel(gunModel);
	camObj->addScript(g);
	camObj->addScript(std::make_shared<PlayerHealthController>());

	camObj->tags.emplace_back("Player");

	GameObjects::addNewObjectToPool(camObj);

	auto gunObj = std::make_shared<GameObject>();
	gunObj->addScript(gunModel);
	GameObjects::addNewObjectToPool(gunObj);

	// - - - !PLAYER - - - 

	// - - - CHAT BOX - - - 
	guiObject = std::make_shared<GameObject>();
	guiObject->transform->position = { 0.f, 0.f, -1.f };
	guiObject->transform->rotation = { 0.0f, 0.0f, 0.0f };
	guiObject->addScript(std::make_shared<ChatBoxText>());
	
	GameObjects::addNewObjectToPool(guiObject);

	// - - - !CHAT BOX - - - 


	// - - - CRATE - - - 
	//crateCube = std::make_shared<GameObject>();
	//crateCube->transform->position = { 0.f, 0.f, -1.f };
	//crateCube->transform->rotation = { 0.0f, 0.0f, 0.0f };
	//crateCube->addScript(std::make_shared<Skybox>());
	//crateCube->addScript(std::make_shared<Cube>());

	//std::shared_ptr<Mover> i = std::make_shared<Mover>();
	//i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	//crateCube->addScript(i);
	
	//GameObjects::addNewObjectToPool(crateCube);
	// - - - !CRATE - - - 

	// - - - SKY BOX - - - 
	skyBox = std::make_shared<GameObject>();
	//skyBox->transform->position = { 0.f, 0.f, -1.f };
	//skyBox->transform->rotation = { 0.0f, 0.0f, 0.0f };
	skyBox->addScript(std::make_shared<Skybox>());
	GameObjects::addNewObjectToPool(skyBox);
	// - - - !SKY BOX - - - 


	// - - - WATER - - - 
	//waterObj = std::make_shared<GameObject>();
	//waterObj->addScript(std::make_shared<billboard2D>());
	//waterObj->transform->rotation = { 90.0f, 0.0f, 0.0f };
	//waterObj->transform->position = { 0.0f, 11.0f, 0.0f };
	//waterObj->transform->scale = { 3000.0f, 3000.0f, 3000.0f };
	//GameObjects::addNewObjectToPool(waterObj);
	// - - - !WATER - - - 


	// - - - GRASS - - -
	//grass = std::make_shared<GameObject>();
	//grass->addScript(std::make_shared<SurfaceGrass>());
	//grass->transform->scale = { 3000.0f, 3000.0f, 3000.0f };
	//GameObjects::addNewObjectToPool(grass);
	// - - - !GRASS - - - 





	// - - - PARTICLE CONTROLLER - - -
	particleController = std::make_shared<GameObject>();
	particleController->addScript(std::make_shared<ParticleController>());
	GameObjects::addNewObjectToPool(particleController);
	// - - - !PARTICLE CONTROLLER - - -




	// - - - ENEMY - - -
	//Enemy = std::make_shared<GameObject>();
	//std::shared_ptr<Mover> t = std::make_shared<Mover>();
	//t->velocity = glm::vec3(1.0f, 0.f, 1.f);
	//Enemy->addScript(t);
	//std::shared_ptr enemyModel = std::make_shared<Model>();
	//std::shared_ptr<ModelPartnerScript> partner = std::make_shared<ModelPartnerScript>();
	//enemyModel->renderPriority = ScriptObject::RenderPriority::noCull;
	//partner->m_pairedModel = enemyModel;
	//Enemy->addScript(partner);
	//Enemy->addScript(enemyModel);
	//Enemy->addScript(std::make_shared<SurfaceFollower>());
	//Enemy->transform->scale = glm::vec3(3.0f);
	//Enemy->tags.emplace_back("interactible");
	//Enemy->tags.emplace_back("shootable");
	//GameObjects::addNewObjectToPool(Enemy);
	// - - - !ENEMY - - - 

	initPlanetSurface();
	initInstancedObjects();
}

int Game::playGame()
{
	double currentFrame = 0.0f;
	double lastFrame = 0.0f;

	while (!Window::Get().windowClosed())
	{
		// update deltatime
		currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame; deltaTime = glm::clamp(deltaTime, 0.0, 5.0);

		Update::update();
		// render as often as possible
		Window::Get().render();
	}
	return 0;
}

void Game::initPlanetSurface()
{
	// initialise
	waterObj = std::make_shared<GameObject>();
	PlanetObj = std::make_shared<GameObject>();

	std::shared_ptr<PlanetSurface> planet = std::make_shared<PlanetSurface>();
	std::shared_ptr<WaterSphere> water = std::make_shared<WaterSphere>();

	Window::Get().PassPlanet(planet, water);

	// Planet

	PlanetObj->transform->position = { 0.0f, 0.0f, 0.0f };
	PlanetObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	PlanetObj->addScript(planet);

	//std::shared_ptr<Mover> t = std::make_shared<Mover>();
	//i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	//PlanetObj->addScript(t);

	GameObjects::addNewObjectToPool(PlanetObj);



	// water

	waterObj->transform->position = { 0.0f, 0.0f, 0.0f };
	waterObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	waterObj->addScript(water);

	//std::shared_ptr<Mover> i = std::make_shared<Mover>();
	//i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	//waterObj->addScript(i);

	GameObjects::addNewObjectToPool(waterObj);

	Window::Get().initPlanet();

	g_planetScript = planet;
}

void Game::initInstancedObjects()
{
	//glm::vec3 planetCenter = g_planetScript->getTransform()->position;
	//
	//
	//SurfaceInstancer instancerTree;
	//
	//InstancerSettings settingsTree;
	//settingsTree.density = 1.0f;
	//settingsTree.noiseScale = 100.0f;
	//settingsTree.noiseThreshold = 1.0f;
	//settingsTree.noiseSeed = rand();
	//settingsTree.useHeightLayerMask = true;
	//settingsTree.heightLayerMask = 2;
	//settingsTree.passesPerFace = 8;
	//
	//instancerTree.SetSettings(settingsTree);
	//
	//treeModel = std::make_shared<Model>();
	//treeModel->loadLocation = "./Assets/Mesh/tree.fbx";
	//treeModel->colour = glm::vec3(143.f, 88.f, 56.f);
	//
	//auto creatorFuncTree = [this]() -> std::shared_ptr<GameObject>
	//	{
	//		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	//
	//		//std::shared_ptr<ModelPartnerScript> m = std::make_shared<ModelPartnerScript>();
	//		//m->m_pairedModel = this->treeModel;
	//		//obj->addScript(m);
	//		obj->addScript(this->treeModel);
	//		obj->addScript(std::make_shared<OrientToCenter>());
	//		obj->transform->rotation = { 270.0f, rand() % 180, 0.0f };
	//		float size = (1.f);
	//		obj->transform->scale = { size , size, size };
	//		return obj;
	//	};
	//
	//instancerTree.InstantiateOnSurface(g_planetScript, creatorFuncTree);


	//SurfaceInstancer instancer2;
	//
	//InstancerSettings settings2;
	//settings2.density = 1.0f;
	//settings2.noiseScale = 100.0f;
	//settings2.noiseThreshold = 1.0f;
	//settings2.noiseSeed = rand();
	//settings2.useHeightLayerMask = true;
	//settings2.heightLayerMask = 1;
	//settings2.passesPerFace = 32;
	//
	//instancer2.SetSettings(settings2);
	//
	//rockModel = std::make_shared<Model>();
	//rockModel->loadLocation = "./Assets/Mesh/rock.fbx";
	//rockModel->colour = glm::vec3(133.f, 133.f, 133.f);
	//
	//auto creatorFunc2 = [this]() -> std::shared_ptr<GameObject>
	//	{
	//		std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
	//
	//		std::shared_ptr<ModelPartnerScript> m = std::make_shared<ModelPartnerScript>();
	//		obj->tags.emplace_back("interactible");
	//
	//		m->m_pairedModel = this->rockModel;
	//		float grayScale = (rand() % 128) + 64.f;
	//
	//		m->colour.x = grayScale;
	//		m->colour.y = grayScale;
	//		m->colour.z = grayScale;
	//
	//		obj->addScript(m);
	//		obj->addScript(this->rockModel);
	//		obj->addScript(std::make_shared<OrientToSurface>());
	//		obj->transform->rotation = { 180.0f, rand() % 180, 0.0f };
	//		float size = ((rand() % 50) / 100.f) + 0.2f;
	//		obj->transform->scale = { size , size, size };
	//		return obj;
	//	};
	//
	//instancer2.InstantiateOnSurface(g_planetScript, creatorFunc2);

	instantiateEnemies();

	initSurfaceGrass();


}






void Game::instantiateEnemies()
{
	

}






void Game::initSurfaceGrass()
{
	SurfaceInstanceHolder::init();
}




// ground tile
	//std::shared_ptr<GameObject> floorObj2 = std::make_shared<GameObject>();
	//floorObj2->transform->position = { 0.0f, -2.0f, -2.0f };
	//floorObj2->transform->scale = { 1.0f, 1.0f, 1.0f };
	//floorObj2->addScript(std::make_shared<GroundTile>());
	//if(floorObj != nullptr) floorObj->active = false;
	//floorObj = floorObj2;
	//GameObjects::addNewObjectToPool(floorObj2);



