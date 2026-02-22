#pragma once

#include "Library.h"
#include <functional>
#include "Camera.h"
#include "KeyScan.h"
#include "PlanetGenHandler.h"
#include <mutex>
#include <atomic>
#include "OnlineDispatcher.h"
#include "Globals.h"

class Window
{
public:
	static Window& Get()
	{
		static Window instance;
		return instance;
	}

	// DELETE FUNCTIONS TO AVOID MORE INSTANCES
	Window(Window const&) = delete;
	void operator=(Window const&) = delete;

    void InitCamera(float fovDeg = 60.0f, float zNear = 0.1f, float zFar = 1000.0f) {
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
        m_camera.setPerspective(fovDeg, aspect, zNear, zFar);
    }
    
    void SetCameraFromTransform(Transform& t);
    
    glm::mat4 GetView() const { return m_camera.GetView(); }
    glm::mat4 GetProj() const { return m_camera.GetProj(); }
    glm::vec3 GetCameraPosition() const { return m_camera.GetPosition(); }
    
    int Width()  const { return m_width; }
    int Height() const { return m_height; }
	
    void Update();
    void ClickIn();
    
	~Window();
	bool windowClosed();
	void render();

    void PassPlanet(std::shared_ptr<PlanetSurface> t_planet, std::shared_ptr<WaterSphere> t_water);
    void passNewPlanetSettings(PlanetPayload t_payload);

    void closeGUI();

    void openGui(std::string t_arguement);

    void initPlanet();
private:
    Window();
    void initGui();
    bool escDown = false;
    bool tabbedOut = false;
    std::shared_ptr<mouseKeyInput> clickIntoWindow;

	GLFWwindow* m_window = nullptr;
    GLFWwindow* guiWindow = nullptr;


    int m_width = WINDOW_SIZE_X;
    int m_height = WINDOW_SIZE_Y;

    Camera m_camera;

    bool guiActive = AUTO_GUI;
    PlanetGenHandler planetGen;

    std::mutex planetMutex;
    PlanetPayload mostRecentPlanet;
    std::atomic<bool> newPlanetWaitingInBuffer = false;
};
