#pragma once
#include "ScriptObject.h"
#include "Window.h"
#include "PlanetSurface.h"

class CameraFeed : public ScriptObject
{
public:
    // height in world units to offset camera above the object's surface
    float cameraHeight = 1.8f;

    void Start() override {
        if (!transform) return;
        Transform camT = *transform;
        // compute up (planet radial if available)
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if (PlanetSurface::s_instance && PlanetSurface::s_instance->getTransform()) {
            up = glm::normalize(transform->position - PlanetSurface::s_instance->getTransform()->position);
        }
        camT.position += up * cameraHeight;
        Window::Get().SetCameraFromTransform(camT);
    }
    void Render() override {
        if (!transform) return;
        Transform camT = *transform;
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if (PlanetSurface::s_instance && PlanetSurface::s_instance->getTransform()) {
            up = glm::normalize(transform->position - PlanetSurface::s_instance->getTransform()->position);
        }
        camT.position += up * cameraHeight;
        Window::Get().SetCameraFromTransform(camT);
    }
};
