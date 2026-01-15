#include "billboard2D.h"
#include "TextureStore.h"

void billboard2D::Start()
{
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

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::litTexture, Shader::FragmentShaderType::litTexture);

    textureID = TextureStore::RetrieveTexture("./Assets/Images/grass.png");


    heightMapID = TextureStore::RetrieveNormals("./Assets/Images/grass.png");

    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    uColourLoc = glGetUniformLocation(m_shader->shaderPair, "colour");
    textureLoc = glGetUniformLocation(m_shader->shaderPair, "uTexture");
    heightLoc = glGetUniformLocation(m_shader->shaderPair, "uHeightMap");
}

void billboard2D::Render()
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

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
