#pragma once
#include "ScriptObject.h"
#include "Window.h"

class CameraFeed : public ScriptObject
{
public:
    void Start() override {
        if (transform) Window::Get().SetCameraFromTransform(*transform);
    }
    void Render() override {
        if (transform) Window::Get().SetCameraFromTransform(*transform);
    }
};
