#include "SurfaceGrass.h"
#include "TextureStore.h"
#include "PlanetSurface.h"

void SurfaceGrass::Start()
{
    renderPriority = RenderPriority::Cull;

    float points[] = {
        // positions          // texture coords
        0.5f,   0.5f,  0.0f,   1.0f, 1.0f,
        0.5f,  -0.5f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.0f,   0.0f, 1.0f,
        0.5f,   0.5f,  0.0f,   1.0f, 1.0f
    };

    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // planet position buffer
    //PlanetSurface::s_instance->GetSurfacePointFromWorldPosition()

    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);



    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::billboard, Shader::FragmentShaderType::billboard);

    textureID = TextureStore::RetrieveTexture("./Assets/Images/grass.png");


    heightMapID = TextureStore::RetrieveNormals("./Assets/Images/grass.png");

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
    textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
    heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
}

void SurfaceGrass::Render()
{
    VertexShaders::LoadShader(m_shader);

    glBindVertexArray(m_body.vao);

    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = Window::Get().GetView();
    glm::mat4 proj = Window::Get().GetProj();

    if (textureLoc != -1 && heightMapID != -1 && heightLoc != -1 && textureID != -1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureLoc, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, heightMapID);
        glUniform1i(heightLoc, 1);
    }

    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
    if (uColourLoc >= 0) glUniform3f(uColourLoc, 1.f, 1.f, 1.f);

    int billboardAmt = 4;
    static const float kYawAngles[6] = { 0.f, 90.f, 45.f, 135.f, 120.f, 150.f };
    static const float kTiltAngles[6] = { 20.f, -30.f,10.f, 3.f, 3.5f, -1.f };
    static const float kHeightScales[6] = { 0.85f, 1.00f, 1.10f, 0.90f, 1.05f, 0.95f };

    glDepthMask(GL_FALSE);
    for (int i = 0; i < billboardAmt; ++i)
    {
        glm::mat4 newModel = model;
        newModel = glm::rotate(newModel, glm::radians(kYawAngles[i]), glm::vec3(0.f, 1.f, 0.f));
        newModel = glm::rotate(newModel, glm::radians(kTiltAngles[i]), glm::vec3(1.f, 0.f, 1.f));
        newModel = glm::scale(newModel, glm::vec3(1.f, kHeightScales[i], 1.f));

        if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(newModel));

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glDepthMask(GL_TRUE);
}
