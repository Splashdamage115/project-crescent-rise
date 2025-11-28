#include "SpherePlanet.h"
#include <cmath>
#include <map>

void SpherePlanet::Start()
{
    generateIcosphere();

    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_body.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::sphereTerrain, Shader::FragmentShaderType::sphereTerrain);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
}

void SpherePlanet::Render()
{
    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();

    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

void SpherePlanet::generateIcosphere()
{
    vertices.clear();
    indices.clear();

    // Generate 3D Voronoi points in two layers (coarse and fine)
    std::vector<glm::vec3> coarseVoronoiPoints;
    std::vector<glm::vec3> fineVoronoiPoints;
    
    generate3DVoronoi(coarseVoronoiPoints, 32);   // Coarse layer
    generate3DVoronoi(fineVoronoiPoints, 128);    // Fine layer

    const float phi = (1.0f + std::sqrt(5.0f)) * 0.5f;
    const float a = 1.0f;
    const float b = 1.0f / phi;

    // generate first 12 vertices
    std::vector<glm::vec3> icosahedronVertices = {
        MathUtils::normalize(glm::vec3(-b,  a,  0)), MathUtils::normalize(glm::vec3(b,  a,  0)),
        MathUtils::normalize(glm::vec3(-b, -a,  0)), MathUtils::normalize(glm::vec3(b, -a,  0)),
        MathUtils::normalize(glm::vec3(0, -b,  a)), MathUtils::normalize(glm::vec3(0,  b,  a)),
        MathUtils::normalize(glm::vec3(0, -b, -a)), MathUtils::normalize(glm::vec3(0,  b, -a)),
        MathUtils::normalize(glm::vec3(a,  0, -b)), MathUtils::normalize(glm::vec3(a,  0,  b)),
        MathUtils::normalize(glm::vec3(-a,  0, -b)), MathUtils::normalize(glm::vec3(-a,  0,  b))
    };

    // link faces
    std::vector<std::vector<unsigned int>> icosahedronFaces = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
    };

    std::vector<glm::vec3> subdivisionVertices = icosahedronVertices;
    std::vector<std::vector<unsigned int>> subdivisionFaces = icosahedronFaces;

    for (int level = 0; level < ICOSPHERE_SUBDIVISIONS; ++level) {
        std::vector<std::vector<unsigned int>> newFaces;
        std::map<std::pair<unsigned int, unsigned int>, unsigned int> midPointCache;

        for (const auto& face : subdivisionFaces) {
            auto getMidPoint = [&](unsigned int i, unsigned int j) -> unsigned int {
                std::pair<unsigned int, unsigned int> key = { std::min(i, j), std::max(i, j) };
                auto it = midPointCache.find(key);
                if (it != midPointCache.end()) {
                    return it->second;
                }

                glm::vec3 midPoint = MathUtils::normalize((subdivisionVertices[i] + subdivisionVertices[j]) * 0.5f);
                subdivisionVertices.push_back(midPoint);
                unsigned int index = static_cast<unsigned int>(subdivisionVertices.size() - 1);
                midPointCache[key] = index;
                return index;
                };

            unsigned int a = face[0], b = face[1], c = face[2];
            unsigned int ab = getMidPoint(a, b);
            unsigned int bc = getMidPoint(b, c);
            unsigned int ca = getMidPoint(c, a);

            newFaces.push_back({ a, ab, ca });
            newFaces.push_back({ b, bc, ab });
            newFaces.push_back({ c, ca, bc });
            newFaces.push_back({ ab, bc, ca });
        }

        subdivisionFaces = newFaces;
    }

    // Create a map to avoid duplicate vertices using the original vertex index
    std::map<unsigned int, unsigned int> vertexIndexMap;

    for (const auto& face : subdivisionFaces) {
        for (unsigned int vertexIndex : face) {
            unsigned int finalIndex;
            
            // Check if we've already processed this original vertex
            auto it = vertexIndexMap.find(vertexIndex);
            if (it != vertexIndexMap.end()) {
                finalIndex = it->second;
            } else {
                // Get normalized vertex position (unit sphere)
                glm::vec3 normalizedVertex = subdivisionVertices[vertexIndex];
                
                // Calculate UV coordinates for texture mapping
                glm::vec2 uv = MathUtils::calculateUV(normalizedVertex);
                
                // Calculate 3D Voronoi displacement using both layers
                float coarseHeight = calculate3DVoronoiHeight(normalizedVertex, coarseVoronoiPoints);
                float fineHeight = calculate3DVoronoiHeight(normalizedVertex, fineVoronoiPoints);
                float combinedHeight = coarseHeight + (fineHeight * 0.5f);
                
                // Apply radial displacement (outward from center)
                float totalRadius = ICOSPHERE_RADIUS + (combinedHeight * displacementScale);
                glm::vec3 displacedVertex = normalizedVertex * totalRadius * ICOSPHERE_SCALE;

                // Add vertex data
                vertices.push_back(displacedVertex.x);
                vertices.push_back(displacedVertex.y);
                vertices.push_back(displacedVertex.z);
                vertices.push_back(uv.x);
                vertices.push_back(uv.y);

                finalIndex = static_cast<unsigned int>(vertices.size() / 5 - 1);
                vertexIndexMap[vertexIndex] = finalIndex;
            }

            indices.push_back(finalIndex);
        }
    }
}

void SpherePlanet::generate3DVoronoi(std::vector<glm::vec3>& voronoiPoints, int pointCount)
{
    voronoiPoints.clear();
    voronoiPoints.reserve(pointCount);

    for (int i = 0; i < pointCount; i++)
    {
        // Generate random points on unit sphere using spherical coordinates
        float u = static_cast<float>(rand()) / RAND_MAX;  // [0, 1]
        float v = static_cast<float>(rand()) / RAND_MAX;  // [0, 1]
        
        // Convert uniform random values to spherical coordinates
        float theta = 2.0f * MathUtils::PI * u;  // Azimuth [0, 2π]
        float phi = std::acos(2.0f * v - 1.0f);  // Polar angle [0, π] (uniform distribution on sphere)
        
        // Convert spherical to Cartesian coordinates
        float sinPhi = std::sin(phi);
        glm::vec3 point(
            sinPhi * std::cos(theta),
            sinPhi * std::sin(theta),
            std::cos(phi)
        );
        
        voronoiPoints.push_back(point);
    }
}

float SpherePlanet::calculate3DVoronoiHeight(const glm::vec3& position, const std::vector<glm::vec3>& voronoiPoints)
{
    if (voronoiPoints.size() < 2) return 0.0f;

    // Find the two closest Voronoi points to the given position
    float closestDistance = glm::distance(position, voronoiPoints[0]);
    float secondClosest = glm::distance(position, voronoiPoints[1]);
    
    // Ensure closestDistance <= secondClosest
    if (closestDistance > secondClosest) {
        std::swap(closestDistance, secondClosest);
    }
    
    // Check all other points
    for (size_t i = 2; i < voronoiPoints.size(); i++)
    {
        float currentDistance = glm::distance(position, voronoiPoints[i]);
        
        if (currentDistance < closestDistance)
        {
            secondClosest = closestDistance;
            closestDistance = currentDistance;
        }
        else if (currentDistance < secondClosest)
        {
            secondClosest = currentDistance;
        }
    }
    
    // Return combined distance scaled by height factor
    // This creates the characteristic Voronoi cell pattern
    return (closestDistance + secondClosest) * heightFactor;
}
