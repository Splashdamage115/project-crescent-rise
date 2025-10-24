#pragma once

#include "RenderableObject.h"

class IcoSphere : public RenderableObject
{
public:
    virtual void Start() override;
    virtual void Render() override;
private:
    void generateIcosphere();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static constexpr float ICOSPHERE_RADIUS = 20.0f;
    static constexpr int ICOSPHERE_SUBDIVISIONS = 3;
    static constexpr float ICOSPHERE_SCALE = 20.0f;
};
