#include "CubeSphere.h"
#include "CubeSphereFace.h"

void CubeSphere::ResetPlanet()
{
    callChange = true;

    shapeGenerator.settings = shapeSettings;
    shapeGenerator.reset();

    std::vector<float> vertices;
    vertices.clear();
    vertices.resize(6 *(pointsPerRow * pointsPerRow) * 8);
    std::vector<unsigned int> indices;
    indices.clear();
    indices.resize(6 * (pointsPerRow-1) * (pointsPerRow-1) * 6);


    std::vector<glm::vec3> direction;

    direction.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));   // Up
    direction.emplace_back(glm::vec3(0.0f, -1.0f, 0.0f));  // Down
    direction.emplace_back(glm::vec3(-1.0f, 0.0f, 0.0f));  // Left
    direction.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));   // Right
    direction.emplace_back(glm::vec3(0.0f, 0.0f, -1.0f));  // Forward
    direction.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f));   // Back

    for (int i = 0; i < 6; i++)
    {
        CubeSphereFace::generateFace(vertices, indices, pointsPerRow, direction.at(i), i, shapeGenerator);
    }
    size = indices.size();

    glBindVertexArray(m_body.vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
}

void CubeSphere::Start()
{
    // Generate and setup VAO
    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_body.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_body.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::lit, Shader::FragmentShaderType::lit);

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    uColourLoc = glGetUniformLocation(m_shader->shaderPair, "baseColour");
    MinMax = glGetUniformLocation(m_shader->shaderPair, "minMax");

    // Generate initial mesh data
    ResetPlanet();
    callChange = false;
}

void CubeSphere::Render()
{
    if (!enabled) return;

    if (callChange) ResetPlanet();

    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();

    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
    if (uColourLoc >= 0) glUniform3f(uColourLoc, (float)planetColour.r, (float)planetColour.g, (float)planetColour.b);
    if (MinMax >= 0) glUniform2f(MinMax, shapeGenerator.elevationMinMax.getMin(), shapeGenerator.elevationMinMax.getMax());


    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}
