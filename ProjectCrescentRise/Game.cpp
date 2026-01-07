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

double Game::deltaTime = 0;

void Game::initGame()
{
	OnlineDispatcher::Init();

	Window::Get();
	Window::Get().InitCamera();
	
	// - - - PLAYER - - - 

	camObj = std::make_shared<GameObject>();
	camObj->transform->position = { 50.f, 0.f, 0.f };
	camObj->transform->rotation = { 0.0f, 0.0f, 0.0f };
	camObj->addScript(std::make_shared<CameraFeed>());
	camObj->addScript(std::make_shared<PlayerInput>());
	camObj->addScript(std::make_shared<SurfaceFollower>());
	//camObj->addScript(std::make_shared<CubeSphere>());

	GameObjects::addNewObjectToPool(camObj);

	// - - - !PLAYER - - - 


	// - - - CHAT BOX - - - 
	guiObject = std::make_shared<GameObject>();
	guiObject->transform->position = { 0.f, 0.f, -1.f };
	guiObject->transform->rotation = { 0.0f, 0.0f, 0.0f };
	guiObject->addScript(std::make_shared<ChatBoxText>());
	
	GameObjects::addNewObjectToPool(guiObject);

	// - - - !CHAT BOX - - - 


	// - - - CRATE - - - 
	crateCube = std::make_shared<GameObject>();
	//crateCube->transform->position = { 0.f, 0.f, -1.f };
	//crateCube->transform->rotation = { 0.0f, 0.0f, 0.0f };
	//crateCube->addScript(std::make_shared<Skybox>());
	crateCube->addScript(std::make_shared<Cube>());

	std::shared_ptr<Mover> i = std::make_shared<Mover>();
	i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	crateCube->addScript(i);
	
	//GameObjects::addNewObjectToPool(crateCube);
	// - - - !CRATE - - - 

	// - - - SKY BOX - - - 
	skyBox = std::make_shared<GameObject>();
	//skyBox->transform->position = { 0.f, 0.f, -1.f };
	//skyBox->transform->rotation = { 0.0f, 0.0f, 0.0f };
	//skyBox->addScript(std::make_shared<Skybox>());
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

	initFloor();

	// - - - RELOADING OF FLOOR - - - 
	//mski = std::make_shared<mouseKeyInput>();
	//mski->active = true;
	//mski->function = { [this]() {this->initFloor(); } };
	//mski->keyCode = KeyScan::MouseKeyCode::RightMouse;
	//KeyScan::append(mski, true);
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

// ground tile
	//std::shared_ptr<GameObject> floorObj2 = std::make_shared<GameObject>();
	//floorObj2->transform->position = { 0.0f, -2.0f, -2.0f };
	//floorObj2->transform->scale = { 1.0f, 1.0f, 1.0f };
	//floorObj2->addScript(std::make_shared<GroundTile>());
	//if(floorObj != nullptr) floorObj->active = false;
	//floorObj = floorObj2;
	//GameObjects::addNewObjectToPool(floorObj2);


void Game::initFloor()
{
	// initialise
	waterObj = std::make_shared<GameObject>();
	PlanetObj = std::make_shared<GameObject>();

	std::shared_ptr<ScriptObject> planet = std::make_shared<PlanetSurface>();
	std::shared_ptr<ScriptObject> water = std::make_shared<WaterSphere>();

	Window::Get().PassPlanet(std::static_pointer_cast<PlanetSurface>(planet), std::static_pointer_cast<WaterSphere>(water));

	// Planet

	PlanetObj->transform->position = { 0.0f, 0.0f, 0.0f };
	PlanetObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	PlanetObj->addScript(planet);

	std::shared_ptr<Mover> t = std::make_shared<Mover>();
	//i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	PlanetObj->addScript(t);

	GameObjects::addNewObjectToPool(PlanetObj);




	// water

	waterObj->transform->position = { 0.0f, 0.0f, 0.0f };
	waterObj->transform->scale = { 1.0f, 1.0f, 1.0f };
	waterObj->addScript(water);

	std::shared_ptr<Mover> i = std::make_shared<Mover>();
	//i->rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	waterObj->addScript(i);

	GameObjects::addNewObjectToPool(waterObj);




}
