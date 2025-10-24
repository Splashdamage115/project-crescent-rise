#include "IcoSphere.h"
#include <cmath>
#include <map>

void IcoSphere::Start()
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

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::Blue);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
}

void IcoSphere::Render()
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

void IcoSphere::generateIcosphere()
{
    vertices.clear();
    indices.clear();

    const float phi = (1.0f + std::sqrt(5.0f)) * 0.5f;
    const float a = 1.0f;
    const float b = 1.0f / phi;

    // generate first 12 vertices
    std::vector<glm::vec3> icosahedronVertices = {
        MathUtils::normalize(glm::vec3(-b,  a,  0)), MathUtils::normalize(glm::vec3( b,  a,  0)),
        MathUtils::normalize(glm::vec3(-b, -a,  0)), MathUtils::normalize(glm::vec3( b, -a,  0)),
        MathUtils::normalize(glm::vec3( 0, -b,  a)), MathUtils::normalize(glm::vec3( 0,  b,  a)),
        MathUtils::normalize(glm::vec3( 0, -b, -a)), MathUtils::normalize(glm::vec3( 0,  b, -a)),
        MathUtils::normalize(glm::vec3( a,  0, -b)), MathUtils::normalize(glm::vec3( a,  0,  b)),
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
                std::pair<unsigned int, unsigned int> key = {std::min(i, j), std::max(i, j)};
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

            newFaces.push_back({a, ab, ca});
            newFaces.push_back({b, bc, ab});
            newFaces.push_back({c, ca, bc});
            newFaces.push_back({ab, bc, ca});
        }

        subdivisionFaces = newFaces;
    }

    std::map<glm::vec3, unsigned int, std::function<bool(const glm::vec3&, const glm::vec3&)>> vertexMap(
        [](const glm::vec3& a, const glm::vec3& b) {
            if (a.x != b.x) return a.x < b.x;
            if (a.y != b.y) return a.y < b.y;
            return a.z < b.z;
        }
    );

    for (const auto& face : subdivisionFaces) {
        for (unsigned int vertexIndex : face) {
            glm::vec3 vertex = subdivisionVertices[vertexIndex] * ICOSPHERE_RADIUS * ICOSPHERE_SCALE;
            
            auto it = vertexMap.find(vertex);
            unsigned int index;
            
            if (it == vertexMap.end()) {
                glm::vec2 uv = MathUtils::calculateUV(MathUtils::normalize(vertex));
                
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
                vertices.push_back(uv.x);
                vertices.push_back(uv.y);
                
                index = static_cast<unsigned int>(vertices.size() / 5 - 1);
                vertexMap[vertex] = index;
            } else {
                index = it->second;
            }
            
            indices.push_back(index);
        }
    }
}
