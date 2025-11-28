#pragma once

#include "RenderableObject.h"

class SpherePlanet : public RenderableObject
{
public:
    virtual void Start() override;
    virtual void Render() override;
private:
    void generateIcosphere();
    void generate3DVoronoi(std::vector<glm::vec3>& voronoiPoints, int pointCount);
    float calculate3DVoronoiHeight(const glm::vec3& position, const std::vector<glm::vec3>& voronoiPoints);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static constexpr float ICOSPHERE_RADIUS = 20.0f;
    static constexpr int ICOSPHERE_SUBDIVISIONS = 7;
    static constexpr float ICOSPHERE_SCALE = 18.0f;

    static constexpr float heightFactor = 1.f;  // Match GroundTile's heightFactor
    static constexpr int width = 1024;            // Resolution for Voronoi sampling
    static constexpr float displacementScale = 2.4f;  // Overall displacement scaling
};
