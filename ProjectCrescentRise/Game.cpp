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
	
}

void Game::initMainMenu()
{
	PlanetSurface::s_instance = nullptr;
	g_planetScript = nullptr;
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
		
	}

	OnlineDispatcher::Init();

	Window::Get().StartGame();
	Window::Get().InitCamera();


	
	overlayObj = std::make_shared<GameObject>();
	auto overlay = std::make_shared<ScreenOverlay>();
	overlay->mainGame = this;
	overlayObj->addScript(overlay);
	overlayObj->tags.emplace_back("overlay");
	GameObjects::addNewObjectToPool(overlayObj);
	
	
	

	std::shared_ptr<Model> gunModel = std::make_shared<Model>();

	auto cameraFeed = std::make_shared<CameraFeed>();

	camObj = std::make_shared<GameObject>();
	camObj->transform->position = { 1050.f, 0.f, 0.f };
	camObj->transform->rotation = { 0.0f, 0.0f, 0.0f };
	camObj->addScript(cameraFeed);
	camObj->addScript(std::make_shared<PlayerInput>());

	auto sf = std::make_shared<SurfaceFollower>();
	sf->floatOnWater = true;
	camObj->addScript(sf);
	

	auto g = std::make_shared<GunController>();
	g->setGunModel(gunModel);
	camObj->addScript(g);
	camObj->addScript(std::make_shared<PlayerHealthController>());

	camObj->tags.emplace_back("Player");

	GameObjects::addNewObjectToPool(camObj);

	auto gunObj = std::make_shared<GameObject>();
	gunObj->addScript(gunModel);
	GameObjects::addNewObjectToPool(gunObj);

	

	
	guiObject = std::make_shared<GameObject>();
	guiObject->transform->position = { 0.f, 0.f, -1.f };
	guiObject->transform->rotation = { 0.0f, 0.0f, 0.0f };
	guiObject->addScript(std::make_shared<ChatBoxText>());
	
	GameObjects::addNewObjectToPool(guiObject);

	


	
	
	
	
	
	

	
	
	
	
	
	

	
	skyBox = std::make_shared<GameObject>();
	
	
	skyBox->addScript(std::make_shared<Skybox>());
	GameObjects::addNewObjectToPool(skyBox);
	


	
	
	
	
	
	
	
	


	
	
	
	
	
	





	
	particleController = std::make_shared<GameObject>();
	particleController->addScript(std::make_shared<ParticleController>());
	GameObjects::addNewObjectToPool(particleController);
	




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	initPlanetSurface();
	initInstancedObjects();
}

int Game::playGame()
{
	double currentFrame = 0.0f;
	double lastFrame = 0.0f;

	while (!Window::Get().windowClosed())
	{
		
		currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame; deltaTime = glm::clamp(deltaTime, 0.0, 5.0);

		Update::update();
		
		Window::Get().render();
	}
	return 0;
}

void Game::initPlanetSurface()
{
	
	waterObj = std::make_shared<GameObject>();
	PlanetObj = std::make_shared<GameObject>();

	std::shared_ptr<PlanetSurface> planet = std::make_shared<PlanetSurface>();
	std::shared_ptr<WaterSphere> water = std::make_shared<WaterSphere>();

	Window::Get().PassPlanet(planet, water);

	

	PlanetObj->transform->position = { 0.0f, 0.0f, 0.0f };
	PlanetObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	PlanetObj->addScript(planet);

	
	
	

	GameObjects::addNewObjectToPool(PlanetObj);



	

	waterObj->transform->position = { 0.0f, 0.0f, 0.0f };
	waterObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	waterObj->addScript(water);

	
	
	

	GameObjects::addNewObjectToPool(waterObj);

	Window::Get().initPlanet();

	g_planetScript = planet;
}

void Game::initInstancedObjects()
{
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

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





	
	
	
	
	
	
	



