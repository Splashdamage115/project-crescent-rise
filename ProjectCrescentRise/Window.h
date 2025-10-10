#pragma once

#include "Library.h"
#include <functional>
#include "Camera.h"

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

    void InitCamera(float fovDeg = 60.0f, float zNear = 0.1f, float zFar = 100.0f) {
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
        m_camera.setPerspective(fovDeg, aspect, zNear, zFar);
    }
    
    void SetCameraFromTransform(const Transform& t) {
        float yaw = glm::radians(t.rotation.y);
        float pitch = glm::radians(t.rotation.x);
    
        glm::vec3 forward;
        forward.x = cos(pitch) * sin(yaw);
        forward.y = sin(pitch);
        forward.z = -cos(pitch) * cos(yaw);
    
        glm::vec3 pos = t.position;
        glm::vec3 target = pos + forward;
        glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    
        m_camera.setPosition(pos);
        m_camera.setTarget(target);
        m_camera.setUp(up);
    }
    
    glm::mat4 GetView() const { return m_camera.GetView(); }
    glm::mat4 GetProj() const { return m_camera.GetProj(); }
    
    int Width()  const { return m_width; }
    int Height() const { return m_height; }
	
	~Window();
	bool windowClosed();
	void render();

private:
	Window();
	GLFWwindow* m_window = nullptr;

    int m_width = 1280;
    int m_height = 720;
    Camera m_camera;
};

